#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char** argv) {
	int* cal_num;
	void* shm = NULL;

	int shmid = shmget((key_t)1234, sizeof(int), 0666 | IPC_CREAT);

	if (shmid == -1) {
		perror("shmget Error");
		return 1;
	}

	shm = shmat(shmid, NULL, 0);
	if (shm == (void*)-1) {
		perror("shmat Error");
		return 1;
	}

	cal_num = (int*)shm;

	while (1) {
		*cal_num += 2;
		sleep(1);

		// TODO: Exit Code
	}

	shmdt(NULL);
	shmctl(shmid, IPC_RMID, 0);
	return 0;
}
