#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
	if (argc < 3)
		errx(1, "Wrong number of arguments passed!");

	int fd = open("run.log", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	int time_limit = argv[1][0] - '0';
	int second_time = 0;
	while (1) {
		int pf[2];
		if (pipe(pf) == -1) {
			close(fd);
			err(2, "Pipe failed!");
		}

		int finish;
		int exit_status;
		pid_t pid = fork();
		if (pid == -1) {
			close(fd);
			err(3, "Fork failed!");
		}
		if (pid == 0) {
			int start = time(NULL);
			close(pf[0]);
			write(pf[1], &start, sizeof(start));
			execvp(argv[2], argv + 2);
		}
		close(pf[1]);
		int status;
		wait(&status);
		finish = time(NULL);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
			exit_status = WEXITSTATUS(status);
		}
		else
			exit_status = 0;

		int start;
		read(pf[0], &start, sizeof(start));
		write(fd, &start, sizeof(start));
		write(fd, " ", 1);
		write(fd, &finish, sizeof(finish));
		write(fd, " ", 1);
		write(fd, &exit_status, sizeof(exit_status));
		write(fd, "\n", 1);

		if (exit_status != 0 && finish - start < time_limit) {
			if (second_time) {
				exit(0);
			}
			else
				second_time = 1;
		}
		else
			second_time = 0;

		close(pf[0]);
	}
}