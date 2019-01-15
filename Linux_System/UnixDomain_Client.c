#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAXLINE 1024

int main(int argc, char** argv) {
	int sockfd;
	char buf[MAXLINE];
	struct sockaddr_un addr;

	if (argc != 2) {
		printf("Usage : %s [socket file name]\n", argv[0]);
		return 1;
	}

	if (access(argv[1], R_OK) == -1) {
		printf("socket File access error\n");
		return 1;
	}

	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

	memset((void*)&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, argv[1], strlen(argv[1]));
	connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));

	while (1) {
		memset(buf, 0, MAXLINE);
		printf("> ");
		fgets(buf, MAXLINE - 1, stdin);

		if (strncmp(buf, "quit\n", 5) == 0)
			break;

		write(sockfd, buf, strlen(buf));
		read(sockfd, buf, MAXLINE);
		printf("Server -> %s\n", buf);
	}

	return 0;
}
