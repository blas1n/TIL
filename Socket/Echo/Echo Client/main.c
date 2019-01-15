#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 3500
#define IP "127.0.0.1"

int main() {
	WSADATA WSAData;
	SOCKADDR_IN addr;
	SOCKET sock;
	char buf[1024];
	int readBytes, len;

	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
		return 1;

	while (TRUE) {
		sock = socket(AF_INET, SOCK_STREAM, 0);

		if (sock == INVALID_SOCKET)
			return 1;

		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(PORT);
		inet_pton(AF_INET, IP, &addr.sin_addr.S_un.S_addr);

		if (connect(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
			puts("connect fail");
			closesocket(sock);
			return 1;
		}

		memset(&buf, 0, sizeof(buf));
		printf("enter messages : ");

		for (len = 0; 1; len++) {
			buf[len] = getchar();

			if (buf[len] == '\n') {
				buf[len++] = '\0';
				break;
			}
		}

		printf("send messages (%d bytes)\n", len);
		send(sock, buf, len, 0);

		for (readBytes = 0; readBytes < len;)
			readBytes += recv(sock, buf + readBytes, len - readBytes, 0);

		printf("recv messages = %s\n\n", buf);

		closesocket(sock);
	}

	WSACleanup();

	return 0;
}