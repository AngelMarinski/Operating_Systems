#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

uint16_t mypow(int num, int degree) {
	uint16_t res = 1;
	for (int i = 0; i < degree; i++) {
		res *= num;
	}
	return res;
}

int main(int argc, char** argv) {
	if (argc != 3)
		errx(1, "Wrong number of arguments passed!");

	int finput = open(argv[1], O_RDONLY);
	if (finput == -1)
		err(2, "Opening failure!");

	int foutput = open(argv[2], O_CREAT | O_WRONLY, 0644);
	if (foutput == -1) {
		close(finput);
		err(2, "Opening failure!");
	}

	uint8_t input;
	uint16_t encode = 0;
	int rs;
	while ((rs = read(finput, &input, sizeof(input))) != 0) {
		if (rs == -1) {
			close(finput);
			close(foutput);
			err(3, "Reading failure!");
		}
		for (int i = 0; i < 8; i++) {
			if ((input & (1 << i)) != 0) {
				uint16_t temp = 1 * mypow(2, i + 1) + 0 * mypow(2, i);
				encode += (temp << i);
			}
			else {
				uint16_t temp = 0 * mypow(2, i + 1) + 1 * mypow(2, i);
				encode += (temp << i);
			}
		}
		write(1, &encode, sizeof(encode));
		fprintf(stderr, "%d", encode);
		if (write(foutput, &encode, sizeof(uint16_t)) != sizeof(uint16_t)) {
			close(finput);
			close(foutput);
			err(4, "Writing failure!");
		}
	}

	close(finput);
	close(foutput);
	exit(0);
}