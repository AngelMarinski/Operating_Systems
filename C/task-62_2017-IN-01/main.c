#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>

int main() {
	int pf[2];
	if (pipe(pf) == -1)
		err(1, "pipe failed!");

	pid_t pid = fork();
	if (pid == -1)
		err(99, "fork failed!");
	if (pid == 0) {
		close(pf[0]);
		dup2(pf[1], 1);
		if (execlp("cat", "cat", "/etc/passwd", (char*)NULL) == -1)
			err(2, "execlp failed!");
	}
	int pf2[2];
	if (pipe(pf2) == -1)
		err(1, "pipe failed!");
	pid_t pid2 = fork();
	if (pid2 == -1)
		err(99, "fork failed!");

	if (pid2 == 0) {
		close(pf[1]);
		close(pf2[0]);
		dup2(pf2[1], 1);
		dup2(pf[0], 0);
		if (execlp("cut", "cut", "-d", ":", "-f7-", (char*)NULL) == -1)
			err(2, "execlp failed!");
	}
	close(pf[0]);
	close(pf[1]);
	int pf3[2];
	if (pipe(pf3) == -1)
		err(1, "pipe failed!");
	pid_t pid3 = fork();
	if (pid3 == -1)
		err(99, "fork failed!");

	if (pid3 == 0) {
		close(pf2[1]);
		close(pf3[0]);
		dup2(pf2[0], 0);
		dup2(pf3[1], 1);
		if (execlp("sort", "sort", "-d", (char*)NULL) == -1)
			err(2, "execlp failed!");
	}
	close(pf2[0]);
	close(pf2[1]);
	int pf4[2];
	if (pipe(pf4) == -1)
		err(1, "pipe failed!");

	pid_t pid4 = fork();
	if (pid4 == -1)
		err(99, "fork failed!");

	if (pid4 == 0) {
		close(pf4[0]);
		close(pf3[1]);
		dup2(pf4[1], 1);
		dup2(pf3[0], 0);
		if (execlp("uniq", "uniq", "-c", (char*)NULL) == -1)
			err(2, "execlp failed!");
	}
	close(pf3[0]);
	close(pf3[1]);
	close(pf4[1]);
	dup2(pf4[0], 0);
	if (execlp("sort", "sort", "-n", "-k1", (char*)NULL) == -1)
		err(2, "execlp failed!");
}