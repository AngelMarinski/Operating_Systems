#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char** argv) {
	char c;
	int rs;

	if (argc == 3 && strcmp(argv[1], "-c") == 0) {
		if (strlen(argv[2]) == 1) {
			int pos = argv[2][0] - '0';
			int counter = 1;
			while ((rs = read(0, &c, sizeof(c))) != 0) {
				if (rs == -1)
					err(1, "Reading failure!");
				if (counter == pos) {
					if (write(1, &c, sizeof(c)) != sizeof(c))
						err(2, "Writing failure!");
				}
				counter++;
			}
		}
		else if (strlen(argv[2]) == 3) {
			int from = argv[2][0] - '0';
			int to = argv[2][2] - '0';
			if (from > to)
				errx(3, "Wrong arguments passed!");
			int counter = 1;
			while ((rs = read(0, &c, 1)) != 0) {
				if (rs == -1)
					err(1, "Reading failure!");
				if (counter >= from && counter <= to) {
					if (write(1, &c, 1) != 1)
						err(2, "Writing failure!");
				}
				counter++;
			}
		}
	}
	else if (strcmp(argv[1], "-d") == 0 && strcmp(argv[3], "-f") == 0 && argc == 5) {
		if (strlen(argv[4]) == 1) {
			int counter = 1;
			int pos = argv[4][0] - '0';
			char delimiter = argv[2][0];
			while ((rs = read(0, &c, 1)) != 0) {
				if (rs == -1)
					err(1, "Reading failure!");
				if (delimiter == c) {
					counter++;
					continue;
				}
				if (pos == counter) {
					if (write(1, &c, 1) != 1)
						err(2, "Writing failure!");
				}
			}
		}
		else if (strlen(argv[4]) == 3) {
			int from = argv[4][0] - '0';
			int to = argv[4][2] - '0';
			int counter = 1;
			char delimiter = argv[2][0];
			while ((rs = read(0, &c, 1)) != 0) {
				if (rs == -1)
					err(1, "Reading failure!");
				if (delimiter == c)
					counter++;
				if (counter >= from && counter <= to) {
					if (write(1, &c, 1) != 1)
						err(2, "Writing failure!");
				}
			}
		}
	}
	exit(0);
}