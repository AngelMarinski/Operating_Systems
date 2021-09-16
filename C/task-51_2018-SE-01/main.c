#include <string.h>
#include <err.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	int rs;
	char c;
	bool flag = false;

	if (strcmp(argv[1], "-d") == 0 && argc == 3) {
		char* to_delete = malloc(strlen(argv[2]) + 1);
		strcpy(to_delete, argv[2]);
		while ((rs = read(0, &c, sizeof(c))) != 0) {
			if (rs == -1)
				err(1, "Reading failure!");
			for (unsigned i = 0; i < strlen(to_delete); i++) {
				if (c == to_delete[i]) {
					flag = true;
					break;
				}
			}
			if (flag) {
				flag = false;
				continue;
			}
			if (write(1, &c, sizeof(c)) != sizeof(c))
				err(2, "Writing failure!");
		}
		free(to_delete);
	}
	else if (strcmp(argv[1], "-s") == 0 && argc == 3) {
		char prev = ' ';
		flag = false;
		char* sub = malloc(strlen(argv[2]) + 1);
		strcpy(sub, argv[2]);
		while ((rs = read(0, &c, sizeof(c))) != 0) {
			if (rs == -1)
				err(1, "Reading failure!");

			if (prev == c) {
				for (unsigned i = 0; i < strlen(sub); i++) {
					if (c == sub[i]) {
						flag = true;
						break;
					}
				}
			}
			prev = c;
			if (flag) {
				flag = false;
				continue;
			}

			if (write(1, &c, sizeof(c)) != sizeof(c))
				err(2, "Writing failure!");
		}
		free(sub);
	}
	else if (argc == 3 && strlen(argv[1]) == strlen(argv[2])) {
		char* from = malloc(strlen(argv[1]) + 1);
		strcpy(from, argv[1]);
		char* to = malloc(strlen(argv[2]) + 1);
		strcpy(to, argv[2]);
		unsigned index;
		flag = false;
		while ((rs = read(0, &c, sizeof(c))) != 0) {
			if (rs == -1)
				err(1, "Reading failure!");

			for (unsigned i = 0; i < strlen(from); i++) {
				if (c == from[i]) {
					flag = true;
					index = i;
					break;
				}
			}
			if (flag) {
				flag = false;
				if (write(1, &to[index], sizeof(to[index])) != sizeof(to[index]))
					err(2, "Writing failure!");
				continue;
			}

			if (write(1, &c, sizeof(c)) != sizeof(c))
				err(2, "Writing failure!");
		}
	}
	else
		errx(3, "Wrong arguments passed!");

	exit(0);
}
