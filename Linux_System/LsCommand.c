#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {
	puts("START");
	execl("/bin/ls", "ls", NULL);
	puts("END");
	return 1;
}
