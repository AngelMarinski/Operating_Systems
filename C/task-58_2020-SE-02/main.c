#include <stdint.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv) {
	if (argc != 3)
		errx(1, "Wrong arguments passed!");

	int scl = open(argv[1], O_RDONLY);
	if (scl == -1)
		err(2, "Opening failure!");

	int sdl = open(argv[2], O_RDONLY);
	if (sdl == -1) {
		close(scl);
		err(2, "Opening failure!");
	}
	int fd = open("high.sdl", O_CREAT | O_WRONLY, 0644);
	if (fd == -1) {
		close(scl);
		close(sdl);
		err(2, "Opening failure!");
	}
	uint8_t level;
	uint16_t buff;
	int rs;
	while ((rs = read(scl, &level, sizeof(level))) != 0) {
		if (rs == -1) {
			close(scl);
			close(sdl);
			close(fd);
			err(3, "Reading failure!");
		}
		rs = read(sdl, &buff, sizeof(buff));
		if (rs == -1) {
			close(scl);
			close(sdl);
			close(fd);
			err(3, "Reading failure!");
		}
		if (level == 1) {
			if (write(fd, &buff, sizeof(buff)) != sizeof(buff)) {
				close(fd);
				close(scl);
				close(sdl);
				err(4, "Writing failure!");
			}
		}
	}

	close(fd);
	close(scl);
	close(sdl);
	exit(0);
}