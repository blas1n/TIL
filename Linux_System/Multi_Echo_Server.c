#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAXLINE (1024)
#define PORT (3500)

int main(int argc, char** argv) {
	int sock, client_sock;
	struct sockaddr_in server_addr, client_addr;
	pid_t pid;
	socklen_t addrlen;
	int readn;
	char buf[MAXLINE];

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket Error: ");
		return 1;
	}

	memset((void*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		perror("Bind Error: ");
		return 1;
	}

	if (listen(sock, 5) == -1) {
		perror("Listen Error: ");
		return 1;
	}
	
	puts("Hello, Echo Server is open");
	signal(SIGCHLD, SIG_IGN);
	
	while (1) {
		addrlen = sizeof(client_addr);
		client_sock = accept(sock, (struct sockaddr*)&client_addr, &addrlen);
		
		if (client_sock == -1) {
			perror("Accept Error: ");
			return 1;
		}

		pid = fork();
		
		if (pid  == 0) {
			memset(buf, 0, MAXLINE);

			while ((readn = read(client_sock, buf, MAXLINE)) > 0) {
				printf("Read Data: %s (%d) : %s\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port, buf);	
				write(client_sock, buf, MAXLINE);
				memset(buf, 0, MAXLINE);
			}

			close(client_sock);
			return 0;
		}

		else if (pid > 0) close(client_sock);

		else {
			perror("Fork Error: ");
			return 1;
		}
	}

	return 0;
}
