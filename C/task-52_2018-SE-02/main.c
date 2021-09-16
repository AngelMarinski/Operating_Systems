#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int cmp(const void* a, const void* b) {
	if (*((uint32_t*)a) < *((uint32_t*)b))
		return -1;
	else if (*((uint32_t*)a) > * ((uint32_t*)b))
		return 1;

	return 0;
}

int main(int argc, char** argv) {
	if (argc != 3)
		errx(1, "Wrong arguments passed!");

	struct stat st;
	if (stat(argv[1], &st) == -1)
		err(2, "Stat failure!");
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		err(3, "Opening failure!");

	uint32_t size = st.st_size / sizeof(uint32_t);
	if (size > 4194304)
		errx(4, "File is too large!");

	uint32_t half = size / 2;

	uint32_t* s1 = malloc(half * sizeof(uint32_t));
	ssize_t rs;


	if ((rs = read(fd, s1, half * sizeof(uint32_t))) != 0) {
		if (rs == -1) {
			close(fd);
			err(5, "Reading failure");
		}
	}


	qsort(s1, half, sizeof(uint32_t), cmp);

	int t1 = open("temp.txt", O_CREAT | O_RDWR, 0644);
	if (t1 == -1) {
		close(fd);
		err(3, "Opening failure!");
	}
	size_t res = write(t1, s1, half * sizeof(uint32_t));
	if (res != half * sizeof(uint32_t)) {
		close(fd);
		close(t1);
		err(6, "Writing failure!");
	}


	free(s1);

	uint32_t rhalf = size - half;
	uint32_t* s2 = malloc(rhalf * sizeof(uint32_t));

	if ((rs = read(fd, s2, rhalf * sizeof(uint32_t))) != 0) {
		if (rs == -1) {
			close(fd);
			close(t1);
			err(5, "Reading failure!");
		}
	}


	qsort(s2, rhalf, sizeof(uint32_t), cmp);

	int t2 = open("temp2.txt", O_CREAT | O_RDWR, 0644);
	if (t2 == -1) {
		close(fd);
		close(t1);
		err(3, "Opening failure!");
	}

	if ((res = write(t2, s2, rhalf * sizeof(uint32_t))) != rhalf * sizeof(uint32_t)) {
		close(fd);
		close(t1);
		close(t2);
		err(6, "Writing failure!");
	}

	free(s2);

	int sorted = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (sorted == -1) {
		close(fd);
		close(t1);
		close(t2);
		err(3, "Opening failure!");
	}
	int rs1;
	int rs2;
	uint32_t b1;
	uint32_t b2;

	lseek(t1, 0, SEEK_SET);
	lseek(t2, 0, SEEK_SET);

	while ((rs1 = read(t1, &b1, sizeof(b1))) > 0 && (rs2 = read(t2, &b2, sizeof(b2))) > 0) {
		if (b1 <= b2) {
			if (write(sorted, &b1, sizeof(b1)) != sizeof(b1)) {
				close(fd);
				close(t1);
				close(t2);
				close(sorted);
				err(6, "Writing failure");
			}
			lseek(t2, -1 * sizeof(b2), SEEK_CUR);
		}
		else if (b1 > b2) {
			if (write(sorted, &b2, sizeof(b2)) != sizeof(b2)) {
				close(fd);
				close(t1);
				close(t2);
				close(sorted);
				err(6, "Writing failure");
			}
			lseek(t1, -1 * sizeof(b1), SEEK_CUR);
		}
	}

	if (rs1 == -1 || rs2 == -1) {
		close(fd);
		close(t1);
		close(t2);
		close(sorted);
		err(5, "Reading failure!");
	}

	if (rs1 == sizeof(b1)) {
		if (write(sorted, &b1, sizeof(b1)) != sizeof(b1)) {
			close(fd);
			close(t1);
			close(t2);
			close(sorted);
			err(6, "Writing failure");
		}
	}

	while ((rs2 = read(t2, &b2, sizeof(b2))) != 0) {
		if (rs2 == -1) {
			close(fd);
			close(t1);
			close(t2);
			close(sorted);
			err(5, "Reading failure!");
		}
		if (write(sorted, &b2, sizeof(b2)) != sizeof(b2)) {
			close(fd);
			close(t1);
			close(t2);
			close(sorted);
			err(6, "Writing failure");
		}
	}

	while ((rs1 = read(t1, &b1, sizeof(b1))) != 0) {
		if (rs1 == -1) {
			close(fd);
			close(t1);
			close(t2);
			close(sorted);
			err(5, "Reading failure!");
		}
		if (write(sorted, &b1, sizeof(b1)) != sizeof(b1)) {
			close(fd);
			close(t1);
			close(t2);
			close(sorted);
			err(6, "Writing failure");
		}
	}

	unlink("temp.txt");
	unlink("temp2.txt");

	close(t1);
	close(t2);
	close(fd);
	close(sorted);

	exit(0);
}