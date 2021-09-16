//not tested but should work
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

struct fail_reader {
	uint32_t id;
	uint16_t useless;
	uint16_t useless2;
	uint32_t start;
	uint32_t end;
};

int main(int argc, char** argv) {
	if (argc != 2)
		errx(1, "Wrong arguments passed!");

	int fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		err(2, "Opening failure!");

	uint32_t counter = 0;
	uint64_t x = 0;
	struct fail_reader buff;
	int rs;
	int index = 0;
	uint32_t users[2048] = { 0 };
	uint32_t time[2048] = { 0 };

	while ((rs = read(fd, &buff, sizeof(buff))) != 0) {
		if (rs == -1) {
			close(fd);
			err(3, "Reading failure!");
		}
		x += buff.end - buff.start;
		users[index] = buff.id;
		time[index] = buff.end - buff.start;
		index++;
		counter++;
	}
	x = x / counter;

	lseek(fd, 0, SEEK_SET);
	uint64_t d = 0;
	uint32_t n = 0;
	while ((rs = read(fd, &buff, sizeof(buff))) != 0) {
		if (rs == -1) {
			close(fd);
			err(3, "Reading failure!");
		}
		d = (buff.end - buff.start - x) * (buff.end - buff.start - x);
		n++;
	}
	d = d / n;

	lseek(fd, 0, SEEK_SET);

	while ((rs = read(fd, &buff, sizeof(buff))) != 0) {
		if (rs == -1) {
			close(fd);
			err(3, "Reading failure!");
		}
		uint32_t max = -1;
		uint64_t time_spent = buff.end - buff.start;
		if (time_spent * time_spent > d) {
			for (int i = 0; i <= index; i++) {
				if (users[i] == buff.id) {
					if (time[i] > max) {
						max = time[i];
					}
				}
			}
		}
		if (write(1, &buff.id, sizeof(buff.id)) != sizeof(buff.id)) {
			close(fd);
			err(4, "Writing failure!");
		}
		if (write(1, &max, sizeof(max)) != sizeof(max)) {
			close(fd);
			err(4, "Writing failure!");
		}
		printf("\n");
	}
}