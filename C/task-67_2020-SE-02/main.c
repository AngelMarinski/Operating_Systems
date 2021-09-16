#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>


int main(int argc, char** argv) {
	if (argc != 3)
		errx(1, "Wrong number of arguments passed!");

	int pf[2];
	if (pipe(pf) == -1)
		err(2, "Pipe failed!");

	pid_t pid = fork();
	if (pid == -1)
		err(3, "fork failed!");

	if (pid == 0) {
		close(pf[0]);
		dup2(pf[1], 1);
		if (execlp("cat", "cat", argv[1], (char*)NULL) == -1)
			err(4, "execlp failed!");
	}

	close(pf[1]);
	int fd = open(argv[2], O_CREAT | O_WRONLY, 0644);
	if (fd == -1)
		err(5, "Opening failure!");

	char c;
	int rs;
	while ((rs = read(pf[0], &c, 1)) > 0) {
		if (c == 0x7D) {
			if ((rs = read(pf[0], &c, 1)) == -1) {
				err(6, "Reading failure!");
			}
			c = c ^ 0x20;
			if (write(fd, &c, 1) != 1)
				err(7, "Writing failure!");
		}
		else if (c == 0x55)
			continue;
	}
	if (rs == -1) {
		close(pf[0]);
		close(fd);
		err(6, "Reading failure!");
	}

	close(pf[0]);
	close(fd);
	exit(0);
}