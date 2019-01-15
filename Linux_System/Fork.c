#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {
	pid_t pid;
	int i = 100;

	puts("START");

	pid = fork();
	puts("Fork");

	if (pid < 0) {
		puts("Fail");
		return 1;
	}

	else if (pid == 0) {
		printf("Child Process : %d\n", getpid());

		while (1) {
			printf("P: %d\n", i++);
			sleep(1);
		}
	}

	else {
		printf("Parent Process : %d\n", getpid());

		while (1) {
			printf("C: %d\n", i++);
			sleep(1);
		}
	}
}
