#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT (3800)
#define MAXLEN (255)

typedef struct _Cal_Data {
	int lhs, rhs;
	char op;
	int result;
	short error;
} CalData;

int main(int argc, char** argv) {
	int sock;
	struct sockaddr_in addr;
	socklen_t addrlen;
	CalData data;

	char msg[MAXLEN];
	int lhs, rhs;

	if (argc != 2) {
		printf("Usage: %s [ipaddress]\n", argv[0]);
		return 1;
	}

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("Error: ");
		return 1;
	}

	memset((void*)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	addr.sin_port = htons(PORT);

	while (1) {
		printf("> ");
		fgets(msg, MAXLEN, stdin);

		if (strncmp(msg, "quit\n", 5) == 0)
			break;

		memset((void*)&data, 0, sizeof(data));
		sscanf(msg, "%d %c %d", &lhs, &data.op, &rhs);

		data.lhs = htonl(lhs);
		data.rhs = htonl(rhs);

		addrlen = sizeof(addr);
		sendto(sock, (void*)&data, sizeof(data), 0, (struct sockaddr*)&addr, addrlen);
		recvfrom(sock, (void*)&data, sizeof(data), 0, (struct sockaddr*)&addr, &addrlen);

		if (ntohs(data.error) == 0)
			printf("%d %c %d = %d\n\n", ntohl(data.lhs), data.op, ntohl(data.rhs), ntohl(data.result));
		else
			printf("Error Code: %d\n\n", ntohs(data.error));
	}

	close(sock);
	return 0;
}
