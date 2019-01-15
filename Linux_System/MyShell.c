#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAXLINE (256)
#define Chop(str) (str[strlen(str) - 1] = 0)

int main(int argc, char** argv) {
	char buf[MAXLINE];
	int procStatus;
	pid_t pid;

	puts("My Shell Ver 1.0");

	while (1) {
		printf("> ");
		memset(buf, 0, MAXLINE);
		fgets(buf, MAXLINE - 1, stdin);

		Chop(buf);

		if (strncmp(buf, "quit", 4) == 0)
			break;

		pid = fork();

		if (pid == 0) {
			if (execl(buf, buf, NULL) == -1) {
				puts("Execl Failure");
				return 1;
			}
		}

		if (pid > 0) wait(&procStatus);
	}

	return 0;
}
