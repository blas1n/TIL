#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

int main(int argc, char** argv) {
	int shmid, semid;

	int* cal_num;
	void* shm = NULL;

	struct sembuf semOpen = { 0, -1, SEM_UNDO };
	struct sembuf semClose = { 0, 1, SEM_UNDO };

	shmid = shmget((key_t)1234, sizeof(int), 0666);
	if (shmid == -1) {
		perror("shmget Error");
		return 1;
	}

	semid = semget((key_t)3477, 0, 0666);
	if (semid == -1) {
		perror("semget Error");
		return 1;
	}

	shm = shmat(shmid, NULL, 0);
	if (shm == (void*)-1) {
		perror("shmat Error");
		return 1;
	}

	cal_num = (int*)shm;

	while (1) {
		int local_var = 0;
		if (semop(semid, &semOpen, 1) == -1)
			perror("semop Error");

		local_var = *cal_num + 1;
		sleep(2);
		*cal_num = local_var;

		printf("%d\n", *cal_num);
		semop(semid, &semClose, 1);
	}

	return 1;
}
