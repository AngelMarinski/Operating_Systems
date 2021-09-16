#include <string.h>
#include <stdlib.h>
#include <err.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char** argv) {
	char buffer;
	int rs;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-") == 0) {
			while ((rs = read(0, &buffer, sizeof(buffer))) != 0) {
				if (rs == -1)
					err(1, "Reading failure!");

				fprintf(stdout, "%c", buffer);
			}
			continue;
		}

		int fd = open(argv[i], O_RDONLY);
		if (fd == -1)
			err(2, "Open failure!");

		while ((rs = read(fd, &buffer, sizeof(buffer))) != 0) {
			if (rs == -1) {
				close(fd);
				err(1, "Reading failure!");
			}
			fprintf(stdout, "%c", buffer);
		}
		close(fd);
	}

	exit(0);
}