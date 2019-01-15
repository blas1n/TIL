#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 3800
#define MAXLEN (255)

typedef struct _Cal_Data {
	int lhs, rhs;
	char op;
	int result;
	short error;
} CalData;

int main(int argc, char** argv) {
	SOCKET sock;
	socklen_t addrlen;
	int result, lhs, rhs;
	PSTR ipBuf = (PSTR)malloc(MAXLEN);
	SOCKADDR_IN addr, client_addr;
	CalData data;
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
		perror("Error: ");
		return 1;
	}

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("Error: ");
		return 1;
	}

	memset((void*)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	inet_pton(AF_INET, INADDR_ANY, &addr.sin_addr.S_un.S_addr);

	addrlen = sizeof(addr);

	if (bind(sock, (SOCKADDR*)&addr, addrlen) == -1) {
		perror("Error: ");
		return 1;
	}

	printf("Hello, Calc_Server is open\n");

	while (1) {
		addrlen = sizeof(client_addr);
		recvfrom(sock, (void*)&data, sizeof(data), 0, (SOCKADDR*)&client_addr, &addrlen);

		memset(ipBuf, 0, sizeof(ipBuf));
		inet_ntop(AF_INET, &client_addr.sin_addr, ipBuf, sizeof(ipBuf));

		printf("Client Info: %s (%d)\n", ipBuf, ntohs(client_addr.sin_port));

		lhs = ntohl(data.lhs);
		rhs = ntohl(data.rhs);

		switch (data.op) {
		case '+':
			result = lhs + rhs;
			break;

		case '-':
			result = lhs - rhs;
			break;

		case '*':
			result = lhs * rhs;
			break;

		case '/':
			if (rhs == 0) {
				data.error = htons(2);
				break;
			}

			result = lhs / rhs;
			break;

		default:
			data.error = htons(1);
			break;
		}

		data.result = htonl(result);
		sendto(sock, (void*)&data, sizeof(data), 0, (SOCKADDR*)&client_addr, addrlen);
	}

	printf("Calc_Server is close, bye\n");
	closesocket(sock);
	WSACleanup();
	return 0;
}