#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char** argv) {
	char str;
	if (argc == 1) {
		int counter = 0;
		int rs;
		while ((rs = read(0, &str, sizeof(str))) != 0) {
			if (rs == -1)
				err(1, "Reading failure!");
			int pf[2];
			if (pipe(pf) == -1)
				err(2, "Pipe failed");

			pid_t pid = fork();

			if (pid > 0) {
				close(pf[0]);
				if (write(pf[1], &str, 1) != 1)
					err(3, "Writing failure!");
				counter++;
				int words = 0;

				while ((rs = read(0, &str, sizeof(str))) != 0) {
					if (rs == -1)
						err(1, "Reading failure");

					if (str == ' ' || str == '\n') {
						words++;
						counter = 0;
						str = ' ';
						if (words >= 2)
							break;
						if (write(pf[1], &str, 1) != 1)
							err(3, "Writing failure!");

						continue;
					}
					counter++;

					if (counter > 4)
						errx(99, "Word larger than 4 symbols!");

					if (write(pf[1], &str, 1) != 1)
						err(3, "Writin failure!");
				}
				close(pf[1]);
				int status;
				wait(&status);
				if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
					err(98, "Something went wrong!");
			}
			else if (pid == 0) {
				close(pf[1]);
				char buff[9] = { '\0' };
				if (read(pf[0], buff, sizeof(buff)) == -1)
					err(1, "Reading failure!");

				if (execlp("echo", "echo", buff, (char*)NULL) == -1)
					err(4, "Execlp failed!");
			}
		}
	}
	else if (argc == 2) {
		int counter = 0;
		int rs;
		while ((rs = read(0, &str, sizeof(str))) != 0) {
			if (rs == -1)
				err(1, "Reading failure!");
			int pf[2];
			if (pipe(pf) == -1)
				err(2, "Pipe failed");

			pid_t pid = fork();

			if (pid > 0) {
				close(pf[0]);
				if (write(pf[1], &str, 1) != 1)
					err(3, "Writing failure!");
				counter++;
				int words = 0;

				while ((rs = read(0, &str, sizeof(str))) != 0) {
					if (rs == -1)
						err(1, "Reading failure");

					if (str == ' ' || str == '\n') {
						words++;
						counter = 0;
						str = ' ';
						if (words >= 2)
							break;
						if (write(pf[1], &str, 1) != 1)
							err(3, "Writing failure!");

						continue;
					}
					counter++;

					if (counter > 4)
						errx(99, "Word larger than 4 symbols!");

					if (write(pf[1], &str, 1) != 1)
						err(3, "Writin failure!");
				}
				close(pf[1]);
				int status;
				wait(&status);
				if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
					err(98, "Something went wrong!");
			}
			else if (pid == 0) {
				close(pf[1]);
				char f1[4] = { '\0' };
				char f2[4] = { '\0' };
				int index = 0;
				char c;
				while ((rs = read(pf[0], &c, 1)) != 0) {
					if (rs == -1)
						err(1, "Reading failure!");
					if (c == ' ') {
						index = 0;
						break;
					}
					f1[index] = c;
					index++;
				}
				while ((rs = read(pf[0], &c, 1)) != 0) {
					if (rs == -1)
						err(1, "Reading failure!");
					if (c == ' ') {
						index = 0;
						break;
					}
					f2[index] = c;
					index++;
				}

				if (strlen(f2) != 0) {
					if (execlp(argv[1], argv[1], f1, f2, (char*)NULL) == -1)
						err(4, "Execlp failed!");
				}
				else {
					if (execlp(argv[1], argv[1], f1, (char*)NULL) == -1)
						err(4, "Execlp failed!");
				}
			}
		}
	}
}