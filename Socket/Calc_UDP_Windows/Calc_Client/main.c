#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT (3800)
#define MAXLEN (255)

typedef struct _Cal_Data {
	int lhs, rhs;
	char op;
	int result;
	short error;
} CalData;

int main(int argc, char** argv) {
	WSADATA wsaData;
	SOCKET sock;
	socklen_t addrlen;
	SOCKADDR_IN addr;
	CalData data;

	char msg[MAXLEN];
	int lhs, rhs;

	if (argc != 2) {
		printf("Usage: %s [ip address]\n", argv[0]);
		return 1;
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
		perror("Error: ");
		return 1;
	}

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
		perror("Error: ");
		return 1;
	}

	memset((void*)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	inet_pton(AF_INET, argv[1], &addr.sin_addr.S_un.S_addr);

	while (1) {
		printf("> ");
		fgets(msg, MAXLEN , stdin);

		if (strncmp(msg, "quit\n", 5) == 0)
			break;

		memset((void*)&data, 0, sizeof(data));
		sscanf(msg, "%d %c %d", &lhs, &data.op, &rhs);
		
		data.lhs = htonl(lhs);
		data.rhs = htonl(rhs);

		addrlen = sizeof(addr);
		sendto(sock, (void*)&data, sizeof(data), 0, (SOCKADDR*)&addr, addrlen);
		recvfrom(sock, (void*)&data, sizeof(data), 0, (SOCKADDR*)&addr, &addrlen);

		if (ntohs(data.error) == 0)
			printf("%d %c %d = %d\n\n", ntohl(data.lhs), data.op, ntohl(data.rhs), ntohl(data.result));
		else
			printf("Error Code: %d\n\n", ntohs(data.error));
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}