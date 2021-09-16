#include <err.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct patch {
	uint16_t offset;
	uint8_t orig;
	uint8_t modif;
};

int main(int argc, char** argv) {
	if (argc != 4)
		errx(1, "Wrong arguments passed!");

	int orig = open(argv[1], O_RDONLY);
	if (orig == -1)
		err(2, "Open failure!");

	int modif = open(argv[2], O_RDONLY);
	if (modif == -1) {
		close(orig);
		err(2, "Open failure!");
	}

	int fd = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd == -1) {
		close(orig);
		close(modif);
		err(2, "Open failure!");
	}

	uint8_t original;
	uint8_t modified;
	uint16_t offset;
	int rs;
	struct patch p;
	while ((rs = read(orig, &original, sizeof(original))) > 0) {
		if (read(modif, &modified, sizeof(modified)) != sizeof(modified)) {
			close(orig);
			close(modif);
			close(fd);
			err(3, "Reading failure!");
		}

		if (modified != original) {
			p.offset = offset;
			p.orig = original;
			p.modif = modified;

			if (write(fd, &p, sizeof(p)) != sizeof(p)) {
				\
					close(orig);
				close(modif);
				close(fd);
				err(4, "Writing failure!");
			}
		}
		offset++;
	}

	if (rs == -1) {
		close(orig);
		close(modif);
		close(fd);
		err(3, "Reading failure!");
	}

	close(orig);
	close(modif);
	close(fd);
	exit(0);
}