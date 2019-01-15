#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAXLINE 1024

int main (int argc, char** argv) {
	int rfd, wfd;
	char buf[MAXLINE];

	mkfifo("/tmp/myfifo_r", S_IRUSR | S_IWUSR);
	mkfifo("/tmp/myfifo_w", S_IRUSR | S_IWUSR);

	if ((rfd = open("/tmp/myfifo_r", O_RDWR)) == -1) {
		perror("Rfd Error: ");
		return 1;
	}

	if ((wfd = open("/tmp/myfifo_w", O_RDWR)) == -1) {
		perror("Wfd Error: ");
		return 1;
	}

	puts("Hello, echo pipe server is on");

	while (1) {
		memset(buf, 0x00, MAXLINE);

		if (read(rfd, buf, MAXLINE) < 0) {
			perror("Read Error: ");
			return 1;
		}

		printf("Read: %s", buf);
		write(wfd, buf, MAXLINE);
		lseek(wfd, 0, SEEK_SET);
	}
}
