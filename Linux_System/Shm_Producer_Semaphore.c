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
	
	union semun {
		int val;
		struct semid_ds* buf;
		unsigned short int* array;
	} sem_union;

	struct sembuf semopen = { 0, -1, SEM_UNDO };
	struct sembuf semclose = { 0, 1, SEM_UNDO };

	shmid = shmget((key_t)1234, sizeof(int), 0666 | IPC_CREAT);
	if (shmid == -1) {
		perror("shmget Error");
		return 1;
	}

	semid = semget((key_t)3477, 1, 0666 | IPC_CREAT);
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

	sem_union.val = 1;

	if (semctl(semid, 0, SETVAL, sem_union) == -1) {
		perror("semctl Error");
		return 1;
	}

	while (1) {
		int local_var = 0;

		if (semop(semid, &semopen, 1) == -1) {
			perror("semop Error");
			return 1;
		}

		local_var = *cal_num + 1;
		sleep(1);
		*cal_num = local_var;

		printf("%d\n", *cal_num);
		semop(semid, &semclose, 1);
	}

	return 1;
}
