#include <err.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

struct header {
	uint32_t magic;
	uint8_t version;
	uint8_t data_version;
	uint16_t count;
	uint32_t useless;
	uint32_t useless2;
};

struct first_version {
	uint16_t offset;
	uint8_t original;
	uint8_t modified;
};

struct second_version {
	uint32_t offset;
	uint16_t original;
	uint16_t modified;
};

int main(int argc, char** argv) {
	if (argc != 4)
		errx(1, "Wrong nubmer of arguments passed!");

	struct header h;

	int patch = open(argv[1], O_RDONLY);
	if (patch == -1)
		err(2, "Opening failure!");

	int f1 = open(argv[2], O_RDONLY);
	if (f1 == -1) {
		close(patch);
		err(2, "Opening failure!");
	}
	int rs = read(patch, &h, sizeof(h));
	if (rs == -1) {
		close(f1);
		close(patch);
		err(3, "Reading failure!");
	}
	if (h.magic != 0xEFBEADDE || h.version != 0x01) {
		close(f1);
		close(patch);
		errx(99, "Version or magic key is not right!");
	}
	int f2 = open(argv[3], O_CREAT | O_WRONLY, 0644);
	if (f2 == -1) {
		close(f1);
		close(patch);
		err(2, "Opening failure!");
	}
	if (h.data_version == 0x00) {
		uint8_t buff;
		while ((rs = read(f1, &buff, sizeof(buff))) != 0) {
			if (rs == -1) {
				close(f1);
				close(f2);
				close(patch);
				err(3, "Reading failure!");
			}
			if (write(f2, &buff, sizeof(buff)) != sizeof(buff)) {
				close(f1);
				close(f2);
				close(patch);
				err(4, "Writing failure!");
			}
		}
		struct first_version v1;
		while ((rs = read(patch, &v1, sizeof(v1))) != 0) {
			if (rs == -1) {
				close(f1);
				close(f2);
				close(patch);
				err(3, "Reading failure!");
			}
			if (lseek(f1, v1.offset, SEEK_SET) == -1) {
				close(f1);
				close(f2);
				close(patch);
				err(98, "Non existing offset");
			}
			int rs = read(f1, &buff, sizeof(buff));
			if (rs == -1) {
				close(f1);
				close(f2);
				close(patch);
				err(3, "Reading failure!");
			}
			if (buff == v1.original) {
				lseek(f2, v1.offset, SEEK_SET);
				if (write(f2, &v1.modified, sizeof(v1.modified)) != sizeof(v1.modified)) {
					close(f1);
					close(f2);
					close(patch);
					err(4, "Writing failure!");
				}
			}
			else {
				close(f1);
				close(f2);
				close(patch);
				err(97, "There is no such a byte in this offset!");
			}
		}
	}
	else if (h.data_version == 0x01) {
		uint16_t buff;
		while ((rs = read(f1, &buff, sizeof(buff))) != 0) {
			if (rs == -1) {
				close(f1);
				close(f2);
				close(patch);
				err(3, "Reading failure!");
			}
			if (write(f2, &buff, sizeof(buff)) != sizeof(buff)) {
				close(f1);
				close(f2);
				close(patch);
				err(4, "Writing failure!");
			}
		}

		struct second_version v2;

		while ((rs = read(patch, &v2, sizeof(v2))) != 0) {
			if (rs == -1) {
				close(f1);
				close(f2);
				close(patch);
				err(3, "Reading failure!");
			}
			if (lseek(f1, v2.offset, SEEK_SET) == -1) {
				close(f1);
				close(f2);
				close(patch);
				err(98, "Non existing offset");
			}
			int rs = read(f1, &buff, sizeof(buff));
			if (rs == -1) {
				close(f1);
				close(f2);
				close(patch);
				err(3, "Reading failure!");
			}
			if (buff == v2.original) {
				lseek(f2, v2.offset, SEEK_SET);
				if (write(f2, &v2.modified, sizeof(v2.modified)) != sizeof(v2.modified)) {
					close(f1);
					close(f2);
					close(patch);
					err(4, "Writing failure!");
				}
			}
			else {
				close(f1);
				close(f2);
				close(patch);
				err(97, "There is no such a byte in this offset!");
			}
		}
	}
	else {
		close(f1);
		close(f2);
		close(patch);
		errx(5, "Version is invalid!");
	}

	close(f1);
	close(f2);
	close(patch);
	exit(0);
}

