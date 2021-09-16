#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

struct read_idx {
	uint16_t offset;
	uint8_t lenght;
	uint8_t useless;
};

int main(int argc, char** argv) {
	if (argc != 5) {
		errx(1, "Wrong arguments passed!");
	}
	struct stat f1;
	if (stat(argv[1], &f1) == -1)
		err(2, "Stat failed!");

	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1)
		err(3, "Opening failure!");

	struct stat idx_st;
	if (stat(argv[2], &idx_st) == -1)
		err(2, "Stat failed!");

	int idx1 = open(argv[2], O_RDONLY);
	if (idx1 == -1) {
		close(fd1);
		err(3, "Opening failure!");
	}

	int fd2 = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd2 == -1) {
		close(fd1);
		close(idx1);
		err(3, "Opening failure!");
	}

	int idx2 = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (idx2 == -1) {
		close(fd1);
		close(idx1);
		close(fd2);
		err(3, "Opening failure!");
	}
	struct read_idx idx;
	int rs;
	while ((rs = read(idx1, &idx, sizeof(idx))) != 0) {
		if (rs == -1) {
			close(fd1);
			close(fd2);
			close(idx1);
			close(idx2);
			err(4, "Reading failure!");
		}
		lseek(fd1, idx.offset, SEEK_SET);
		uint8_t* buff = malloc(idx.lenght);
		if ((rs = read(fd1, buff, sizeof(buff))) != 0) {
			if (rs == -1) {
				close(fd1);
				close(fd2);
				close(idx1);
				close(idx2);
				err(4, "Reading failure!");
			}
			if (buff[0] >= 0x41 && buff[0] <= 0x5A) {
				if (write(fd2, buff, sizeof(buff)) != sizeof(buff)) {
					close(fd1);
					close(fd2);
					close(idx1);
					close(idx2);
					err(5, "Writing failure!");
				}
				if (write(idx2, &idx, sizeof(idx)) != sizeof(idx)) {
					close(fd1);
					close(fd2);
					close(idx1);
					close(idx2);
					err(5, "Writing failure!");
				}
			}
		}
		free(buff);
	}

	close(fd1);
	close(fd2);
	close(idx1);
	close(idx2);
	exit(0);
}