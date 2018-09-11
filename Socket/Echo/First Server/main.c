#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define MAX_PACKETLEN 1024
#define PORT 3500

int main() {
	WSADATA wsaData;
	SOCKET server_sock, client_sock;
	SOCKADDR_IN server_addr, client_addr;
	char buf[MAX_PACKETLEN];
	int readn, writen, len;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 1;

	server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (server_sock == INVALID_SOCKET)
		return 1;

	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(PORT);
	inet_pton(AF_INET, INADDR_ANY, &server_addr.sin_addr.S_un.S_addr);

	if (bind(server_sock, (SOCKADDR*)&server_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
		return 0;

	if (listen(server_sock, 5) == SOCKET_ERROR)
		return 0;

	while (TRUE) {
		memset(&client_addr, 0, sizeof(SOCKADDR_IN));
		len = sizeof(SOCKADDR_IN);
		client_sock = accept(server_sock, (SOCKADDR*)&client_addr, &len);

		readn = recv(client_sock, buf, MAX_PACKETLEN, 0);
		printf("recv message: %s\n", buf);

		if (readn > 0)
			writen = send(client_sock, buf, readn, 0);

		closesocket(client_sock);
	}

	closesocket(server_sock);
	WSACleanup();

	return 0;
}