#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <err.h>
#include <errno.h>

int main(int argc, char** argv) {
	if (argc != 4)
		errx(1, "Wrong arguments passed!");

	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1)
		err(2, "Opening failure!");

	int fd2 = open(argv[2], O_RDONLY);
	if (fd2 == -1) {
		close(fd1);
		err(2, "Opening failure!");
	}

	int fd3 = open(argv[3], O_WRONLY);
	if (fd3 == -1) {
		close(fd1);
		close(fd2);
		err(2, "Opening failure!");
	}

	uint32_t offset;
	uint32_t lenght;
	int rs;
	while ((rs = read(fd1, &offset, sizeof(offset))) != 0) {
		if (rs == -1) {
			close(fd1);
			close(fd2);
			close(fd3);
			err(3, "Reading failure!");
		}
		if (read(fd1, &lenght, sizeof(lenght)) == -1) {
			close(fd1);
			close(fd2);
			close(fd3);
			err(3, "Reading failure!");
		}

		uint32_t* buffer = malloc(lenght);
		lseek(fd2, offset, SEEK_SET);
		if ((rs = read(fd2, buffer, sizeof(buffer))) != 0) {
			if (rs == -1) {
				close(fd1);
				close(fd2);
				close(fd3);
				err(3, "Reading failure!");
			}
		}

		if (write(fd3, buffer, sizeof(buffer)) != sizeof(buffer)) {
			close(fd1);
			close(fd2);
			close(fd3);
			err(4, "Writing failure!");
		}
		free(buffer);
	}

	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}