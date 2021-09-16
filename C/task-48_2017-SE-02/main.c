#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <err.h>

int main(int argc, char** argv) {
	if (argc == 1) {
		char buff;
		int rs;
		while ((rs = read(0, &buff, sizeof(buff))) != 0) {
			if (rs == -1) {
				err(1, "Reading failure");
			}
			setbuf(stdout, NULL);
			if (write(1, &buff, sizeof(buff)) != sizeof(buff)) {
				err(2, "Writing failure");
			}
		}
	}
	else {
		if (strcmp(argv[1], "-n") == 0) {
			int line = 1;
			char buff;
			int rs;
			int newline = 1;
			for (int i = 2; i < argc; i++) {

				if (strcmp(argv[i], "-") == 0) {
					while ((rs = read(0, &buff, sizeof(buff))) != 0) {
						if (rs == -1) {
							err(1, "Reading failure");
						}
						if (newline)
							printf("%d ", line);

						setbuf(stdout, NULL);
						if (write(1, &buff, sizeof(buff)) != sizeof(buff)) {
							err(2, "Writing failure");
						}
						if (buff == '\n') {
							newline = 1;
							line++;
						}
						else
							newline = 0;
					}
					continue;
				}

				int fd = open(argv[i], O_RDONLY);
				if (fd == -1) {
					err(3, "Open failure");
				}

				while ((rs = read(fd, &buff, sizeof(buff))) != 0) {
					if (rs == -1) {
						int errno_cpy = errno;
						close(fd);
						errno = errno_cpy;
						err(1, "Reading failure");
					}
					if (newline)
						printf("%d ", line);

					setbuf(stdout, NULL);
					if (write(1, &buff, sizeof(buff)) != sizeof(buff)) {
						int errno_cpy = errno;
						close(fd);
						errno = errno_cpy;
						err(2, "Writing failure");
					}
					if (buff == '\n') {
						newline = 1;
						line++;
					}
					else
						newline = 0;
				}
				close(fd);
			}
		}
		else {
			int rs;
			char buff;
			for (int i = 1; i < argc; i++) {
				if (strcmp(argv[i], "-") == 0) {
					while ((rs = read(0, &buff, sizeof(buff))) != 0) {
						if (rs == -1) {
							err(1, "Reading failure");
						}
						setbuf(stdout, NULL);
						if (write(1, &buff, sizeof(buff)) != sizeof(buff)) {
							err(2, "Writing failure");
						}
					}
					continue;
				}
				int fd = open(argv[i], O_RDONLY);
				if (fd == -1)
					err(3, "Open failure");

				while ((rs = read(fd, &buff, sizeof(buff))) != 0) {
					if (rs == -1) {
						int errno_cpy = errno;
						close(fd);
						errno = errno_cpy;
						err(1, "Reading failure");
					}
					setbuf(stdout, NULL);
					if (write(1, &buff, sizeof(buff)) != sizeof(buff)) {
						int errno_cpy = errno;
						close(fd);
						errno = errno_cpy;
						err(2, "Writing failure");
					}
				}

				close(fd);
			}
		}
	}

	exit(0);
}