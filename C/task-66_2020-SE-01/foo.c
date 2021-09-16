#include <sys/types.h>
#include <sys/stat.h>
#include <err.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv) {
	if (argc != 2)
		errx(1, "Wrong number of arguments passed!");

	if (mkfifo("../66-fifo", 0644) == -1)
		err(99, "mkfifo failed!");

	int fd = open("../66-fifo", O_WRONLY);
	if (fd == -1)
		err(2, "Opening failure!");

	dup2(fd, 1);

	if (execlp("cat", "cat", argv[1], (char*)NULL) == -1) {
		close(fd);
		err(3, "Execlp failed!");
	}
}