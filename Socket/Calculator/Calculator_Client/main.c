#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 3600
#define IP "127.0.0.1"

#define RESETBUF() getchar()

typedef struct _Calc_Data {
	int lhs;
	int rhs;
	char op;
	int result;
	short error;
} CalcData;

int main(int argc, char** argv) {
	WSADATA WSAData;
	SOCKADDR_IN addr;
	SOCKET sock;
	int sbyte, rbyte, len;
	CalcData data;

	if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
		return 1;

	while (1) {
		memset(&data, 0x00, sizeof(data));

		printf("수식을 입력하세요 (ex. 12 + 15). ");
		scanf("%d", &data.lhs);
		RESETBUF();
		data.op = getchar();
		RESETBUF();
		scanf("%d", &data.rhs);

		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (sock == INVALID_SOCKET)
			return 1;

		addr.sin_family = AF_INET;
		inet_pton(AF_INET, IP, &addr.sin_addr.S_un.S_addr);
		addr.sin_port = htons(PORT);

		if (connect(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
			printf("fail to connect\n");
			closesocket(sock);
			return 1;
		}

		len = sizeof(data);

		data.lhs = htonl(data.lhs);
		data.rhs = htonl(data.rhs);
		sbyte = send(sock, (void*)&data, len, 0);

		if (sbyte != len)
			return 1;

		rbyte = recv(sock, (void*)&data, len, 0);
		if (rbyte != len)
			return 1;

		if (ntohs(data.error) != 0)
			printf("Calc Error : %d\n", ntohs(data.error));

		else
			printf("%d %c %d = %d\n", ntohl(data.lhs), data.op, ntohl(data.rhs), ntohl(data.result));

		closesocket(sock);
	}

	WSACleanup();
	return 0;
}