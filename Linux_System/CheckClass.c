#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

#define CLASSMASK htonl(15 << 4)

int main(int argc, char** argv) {
	char buf[256];
	char className[6][16] = {{"E class"}, {"D class"}, {"C class"}, {"B class"}, {"A class"}};

	unsigned int laddr, haddr;
	int i;

	if (argc != 2) {
		fprintf(stderr, "Usage : %s [internet address]\n", argv[0]);
		return 1;
	}

	laddr = inet_addr(argv[1]);

	if (laddr == INADDR_ANY) {
		printf("INADDR ANY\n");
		return 0;
	}

	else if (laddr == INADDR_NONE) {
		printf("INADDR NONE\n");
		return 0;
	}

	haddr = ntohl(laddr);

	for (i = 0; i < 5; i++) {
		if (((CLASSMASK << i) & (haddr)) == (CLASSMASK << i))
			break;
	}

	printf("%s -> %s\n", argv[1], className[i]);
	return 0;
}
