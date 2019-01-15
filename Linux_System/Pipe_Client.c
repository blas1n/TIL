#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAXLINE 1024

int main (int argc, char** argv) {
	int rfd, wfd;
	char buf[MAXLINE];

	if ((rfd = open("/tmp/myfifo_w", O_RDWR)) == -1) {
		perror("Read open error: ");
		return 1;
	}

	if ((wfd = open("/tmp/myfifo_r", O_RDWR)) == -1) {
		perror("Write open error: ");
		return 1;
	}

	while (1) {
		printf("> ");
		fflush(stdout);
		memset(buf, 0, MAXLINE);

		if (read(0, buf, MAXLINE) == -1) {
			perror("Read error: ");
			return 1;
		}

		if (strncmp(buf, "quit\n", 5) == 0)
			break;

		write(wfd, buf, strlen(buf));
		read(rfd, buf, MAXLINE);
		printf("Server -> %s", buf);
	}

	close(wfd);
	close(rfd);
	return 0;
}
