#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

#define MAXLINE 256

int main(int argc, char** argv) {
	char buf[MAXLINE];
	struct in_addr laddr;
	unsigned int ipaddr = 0;

	if (argc != 2) {
		fprintf(stderr, "Usage : %s [internet address]\n", argv[0]);
		return 1;
	}

	ipaddr = inet_addr(argv[1]);
	printf("inet_addr() : %s -> %d\n", argv[1], ipaddr);
	printf("inet_ntoa() : %d -> %s\n", ipaddr, inet_ntoa(*(struct in_addr*)&ipaddr));

	if (inet_aton(argv[1], &laddr) != -1) {
		printf("inet_aton() : %s -> %d\n", argv[1], laddr.s_addr);
		printf("inet_ntoa() : %d -> %s\n", laddr.s_addr, inet_ntoa(*(struct in_addr*)&laddr.s_addr));
	}
	
	else
		perror("Error");

	return 1;
}
