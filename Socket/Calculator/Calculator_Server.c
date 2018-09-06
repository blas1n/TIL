#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termio.h>

#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 3600

typedef struct _Calc_Data {
	int lhs;
	int rhs;
	char op;
	int result;
	short error;
} CalcData;

char getch();

int main(int argc, char** argv) {
	struct sockaddr_in client_addr, sock_addr;
	int listen_sock, client_sock;
	int addr_len;
	CalcData data;
	int lhs, rhs, result;

	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Error");
		return 1;
	}

	memset((void*)&sock_addr, 0x00, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	sock_addr.sin_port = htons(PORT);

	if (bind(listen_sock, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == -1) {
		perror("Errorr");
		return 1;
	}

	if (listen(listen_sock, 5) == -1) {
		perror("Error");
		return 1;
	}

	puts("Hello, Calculator Server is on");

	while (1) {
		addr_len = sizeof(client_addr);
		client_sock = accept(listen_sock, (struct sockaddr*)&client_addr, &addr_len);

		if (client_sock == -1) {
			perror("Error");
			return 1;
		}

		printf("%d is connect\n", client_addr.sin_addr.s_addr);

		read(client_sock, (void*)&data, sizeof(data));
		data.error = 0;

		lhs = ntohl(data.lhs);
		rhs = ntohl(data.rhs);

		switch(data.op) {
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
					data.error = 2;
					break;
				}

				result = lhs / rhs;
				break;

			default:
				data.error = 1;
				break;
		}

		data.result = htonl(result);
		data.error = htons(data.error);

		write(client_sock, (void*)&data, sizeof(data));
		close(client_sock);
	}

	puts("Calculator Server is off. Bye");
	close(listen_sock);
	return 0;
}
