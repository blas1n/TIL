#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv) {
	struct rlimit rlim;

	getrlimit(RLIMIT_NOFILE, &rlim);
	printf("Open file %d : %d\n", (int)rlim.rlim_cur, (int)rlim.rlim_max);

	rlim.rlim_cur += 1024;
	rlim.rlim_max = 65536;

	if (setrlimit(RLIMIT_NOFILE, &rlim) == -1) {
		printf("out");
		return 0;
	}

	printf("Open file %d : %d\n", (int)rlim.rlim_cur, (int)rlim.rlim_max);
	return 0;
}
