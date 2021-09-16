#include<err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

struct patch_read {
	uint16_t offset;
	uint8_t original;
	uint8_t modified;
};

int main(int argc, char** argv) {
	if (argc != 4)
		errx(1, "Wrong arguments passed!");

	int p = open(argv[1], O_RDONLY);
	if (p == -1)
		err(1, "Open failure!");

	int f1 = open(argv[2], O_RDONLY);
	if (f1 == -1) {
		close(p);
		err(1, "Open failure!");
	}

	int f2 = open(argv[3], O_RDWR);
	if (f2 == -1) {
		close(p);
		close(f1);
		err(1, "Open failure!");
	}

	uint8_t buffer;
	int rs;

	while ((rs = read(f1, &buffer, sizeof(buffer))) != 0) {
		if (rs == -1) {
			close(p);
			close(f1);
			close(f2);
			err(2, "Reading failure!");
		}

		if (write(f2, &buffer, sizeof(buffer)) != sizeof(buffer)) {
			close(p);
			close(f1);
			close(f2);
			err(3, "Writing failure!");
		}
	}

	lseek(f1, 0, SEEK_SET);
	lseek(f2, 0, SEEK_SET);
	struct patch_read patch;
	uint8_t reviewer;
	while ((rs = read(p, &patch, sizeof(patch))) != 0) {
		if (rs == -1) {
			close(p);
			close(f1);
			close(f2);
			err(2, "Reading failure!");
		}

		lseek(f1, patch.offset, SEEK_SET);
		rs = read(f1, &reviewer, sizeof(reviewer));
		if (rs == -1 || rs == 0) {
			close(p);
			close(f1);
			close(f2);
			err(2, "Reading failure!");
		}
		if (reviewer != patch.original) {
			close(p);
			close(f1);
			close(f2);
			err(99, "There is no suck original byte!");
		}

		lseek(f2, patch.offset, SEEK_SET);
		if (write(f2, &patch.modified, sizeof(patch.modified)) != sizeof(patch.modified)) {
			close(p);
			close(f1);
			close(f2);
			err(3, "Writing failure!");
		}
	}

	close(p);
	close(f1);
	close(f2);
	exit(0);
}