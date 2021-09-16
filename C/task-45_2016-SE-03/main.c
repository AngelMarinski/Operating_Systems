#include <err.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


int cmp(const void* a, const void* b) {
	if (*((uint32_t*)a) > * ((uint32_t*)b))
		return 1;
	else if (*((uint32_t*)a) < *((uint32_t*)b))
		return -1;

	return 0;
}

int main(int argc, char** argv) {
	if (argc != 2)
		errx(1, "Wrong number of arguments passed!");

	struct stat st;
	if (stat(argv[1], &st) == -1)
		err(2, "Error while running stat!");

	uint32_t fsize = st.st_size / sizeof(uint32_t);
	if (fsize == 0)
		errx(3, "File is empty!");

	uint32_t size = fsize / 2;
	uint32_t* buffer = malloc(size * sizeof(uint32_t));

	int fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		free(buffer);
		err(4, "Opening failure!");
	}

	size_t rs = read(fd, buffer, sizeof(buffer));
	if (rs != size * sizeof(uint32_t)) {
		free(buffer);
		close(fd);
		err(5, "Reading failure");
	}

	int temp = open("temp.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (temp == -1) {
		free(buffer);
		close(fd);
		err(4, "Opening failure!");
	}

	qsort(buffer, size, sizeof(uint32_t), cmp);

	if (write(temp, buffer, sizeof(buffer)) != sizeof(buffer)) {
		free(buffer);
		close(fd);
		close(temp);
		err(6, "Writing failure!");
	}

	free(buffer);

	int temp2 = open("temp2.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (temp2 == -1) {
		close(fd);
		close(temp);
		err(4, "Opening failure!");
	}

	uint32_t* buff2 = malloc(size * sizeof(uint32_t));
	rs = read(fd, buff2, sizeof(buff2));
	if (rs != size * sizeof(uint32_t)) {
		close(fd);
		close(temp);
		close(temp2);
		free(buff2);
		err(5, "Reading failure!");
	}
	qsort(buff2, size, sizeof(uint32_t), cmp);
	if (write(temp2, buff2, sizeof(buff2)) != sizeof(buff2)) {
		close(fd);
		close(temp);
		close(temp2);
		free(buff2);
		err(6, "Writing failure!");
	}
	free(buff2);

	lseek(temp, 0, SEEK_SET);
	lseek(temp2, 0, SEEK_SET);

	uint32_t a;
	uint32_t b;
	int rs1;
	int rs2;
	int sorted = open("sorted.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	while ((rs1 = read(temp, &a, sizeof(a))) == sizeof(a) && (rs2 = read(temp2, &b, sizeof(b))) == sizeof(b)) {
		if (a <= b) {
			if (write(sorted, &a, sizeof(a)) != sizeof(a)) {
				close(fd);
				close(temp);
				close(temp2);
				close(sorted);
				err(6, "Writing failure!");
			}
			lseek(temp2, -1 * sizeof(b), SEEK_CUR);
		}
		else {
			if (write(sorted, &a, sizeof(b)) != sizeof(b)) {
				close(fd);
				close(temp);
				close(temp2);
				close(sorted);
				err(6, "Writing failure!");
			}
			lseek(temp2, -1 * sizeof(a), SEEK_CUR);
		}
	}

	if (rs1 == -1 || rs2 == -1) {
		close(fd);
		close(temp);
		close(temp2);
		close(sorted);
		err(5, "Reading failure!");
	}

	if (rs1 == sizeof(a)) {
		if (write(sorted, &a, sizeof(a)) != sizeof(a)) {
			close(fd);
			close(temp);
			close(temp2);
			close(sorted);
			err(6, "Writing failure!");
		}
	}

	if (rs1 == 0) {
		while ((rs2 = read(temp2, &b, sizeof(b))) == sizeof(b)) {
			if (write(sorted, &b, sizeof(b)) != sizeof(b)) {
				close(fd);
				close(temp);
				close(temp2);
				close(sorted);
				err(6, "Writing failure!");
			}
		}
	}
	else if (rs2 == 0) {
		while ((rs1 = read(temp, &a, sizeof(a))) == sizeof(a)) {
			if (write(sorted, &a, sizeof(a)) != sizeof(a)) {
				close(fd);
				close(temp);
				close(temp2);
				close(sorted);
				err(6, "Writing failure!");
			}
		}
	}

	if (rs1 == -1 || rs2 == -1) {
		close(fd);
		close(temp);
		close(temp2);
		close(sorted);
		err(5, "Reading failure!");
	}

	unlink("temp.txt");
	unlink("temp2.txt");

	close(temp);
	close(temp2);
	close(fd);
	close(sorted);
	exit(0);
}