#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	while (1) {
		write(1, "Command: ", 9);
		int pf[2];
		char buff[100];
		char c;
		int rs;
		if (pipe(pf) == -1)
			err(1, "Pipe failed!");

		pid_t pid = fork();
		if (pid > 0) {
			close(pf[0]);
			int status;
			while ((rs = read(0, &c, 1)) != 0) {
				if (rs == -1)
					err(2, "Reading failure!");
				if (c == '\n') {
					close(pf[1]);
					wait(&status);
					if (WIFEXITED(status) && WEXITSTATUS(status) == 1) {
						exit(0);
					}
					else
						break;
				}
				else
					write(pf[1], &c, 1);
			}
		}
		else if (pid == 0) {
			close(pf[1]);
			if (read(pf[0], buff, sizeof(buff)) == -1)
				err(2, "Reading failure!");

			if (strcmp(buff, "exit") == 0) {
				exit(1);
			}

			if (execlp(buff, buff, (char*)NULL) == -1)
				err(3, "execlp failed!");
		}
	}
}