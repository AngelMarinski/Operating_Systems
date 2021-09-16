#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>

int main(int argc, char** argv) {
	if (argc != 2)
		errx(1, "Wrong number of arguments passed!");

	int fd = open("../66-fifo", O_RDONLY);
	if (fd == -1)
		err(2, "Opening failure!");

	dup2(fd, 0);
	if (execlp(argv[1], argv[1], (char*)NULL) == -1)
		err(3, "Execlp failed!");
}