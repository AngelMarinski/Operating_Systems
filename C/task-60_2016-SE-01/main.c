#include <unistd.h>
#include <err.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char** argv) {
	if (argc != 2)
		errx(1, "Wrong number of arguments passed!");

	int pf[2];
	if (pipe(pf) == -1)
		err(2, "Pipe failed!");
	pid_t pid = fork();


	if (pid == 0) {
		close(pf[0]);
		dup2(pf[1], 1);
		if (execlp("cat", "cat", argv[1], (char*)NULL) == -1) {
			err(3, "Execlp failed!");
		}
	}
	else if (pid > 0) {
		close(pf[1]);
		dup2(pf[0], 0);
		if (execlp("sort", "sort", (char*)NULL) == -1)
			err(3, "Execlp failed!");
	}
	else
		err(4, "Fork failed!");
}