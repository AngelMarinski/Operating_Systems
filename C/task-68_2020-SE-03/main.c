#include <stdint.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

struct read_file {
	char filename[8];
	uint32_t offset;
	uint32_t lenght;
};

int main(int argc, char** argv) {
	if (argc != 2)
		errx(1, "Wrong number of arguments passed!");

	int fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		err(2, "opening failure!");

	struct read_file reader;
	int rs;
	uint16_t parent_res = 0x00;
	while ((rs = read(fd, &reader, sizeof(reader))) > 0) {
		int pf[2];
		if (pipe(pf) == -1)
			err(3, "pipe failed!");

		pid_t pid = fork();
		if (pid == 0) {
			close(pf[0]);

			int file = open(reader.filename, O_RDONLY);

			lseek(file, reader.offset, SEEK_SET);

			uint16_t* buff = malloc(reader.lenght * sizeof(uint16_t));

			if (read(file, buff, sizeof(uint16_t) * reader.lenght) == -1) {
				close(file);
				close(fd);
				close(pf[1]);
				err(4, "reading failure!");
			}
			uint16_t result = 0;
			for (unsigned i = 0; i < reader.lenght; i++) {
				if (buff[i] == 0x00)
					break;

				result = result ^ buff[i];
			}

			free(buff);

			if (write(pf[1], &result, sizeof(result)) != sizeof(result)) {
				close(file);
				close(fd);
				close(pf[1]);
				err(5, "writing failure!");
			}
			close(pf[1]);
			close(file);
			exit(0);
		}
		if (pid > 0) {
			close(pf[1]);
			int status;
			wait(&status);
			uint16_t get_res;
			if (read(pf[0], &get_res, sizeof(get_res)) == -1) {
				close(fd);
				close(pf[0]);
			}
			parent_res = parent_res ^ get_res;
			close(pf[0]);
		}
	}

	fprintf(stdout, "result: %dB\n", parent_res);
}