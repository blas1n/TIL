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
	int sock, result, lhs, rhs;
	socklen_t addrlen;
	struct sockaddr_in addr, client_addr;
	CalData data;

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("Error: ");
		return 1;
	}

	memset((void*)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);

	addrlen = sizeof(addr);
	
	if (bind(sock, (struct sockaddr*)&addr, addrlen) == -1) {
		perror("Error: ");
		return 1;
	}

	printf("Hello, Calc_Server is open\n");

	while (1) {
		addrlen = sizeof(client_addr);
		recvfrom(sock, (void*)&data, sizeof(data), 0, (struct sockaddr*)&client_addr, &addrlen);

		printf("Client Info: %s (%d)\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

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
		sendto(sock, (void*)&data, sizeof(data), 0, (struct sockaddr*)&client_addr, addrlen);
	}

	printf("Calc_Server is close, bye\n");
	close(sock);
	return 0;
}
