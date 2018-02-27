#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <semaphore.h>
#include "queue.h"

Queue* queue;
int shm, sem;

void removing(){
	shmdt(queue);
	shmctl(sem, 0, IPC_RMID);
	shmctl(shm, IPC_RMID, NULL);
}

void handleSig(int sig){
	printf("SIGINT!\n");
	exit(0);
}

void sem_down (int semID, unsigned short semNumber, short semFlags, struct sembuf *sembufStruct){
	sembufStruct->sem_num = semNumber;
	sembufStruct->sem_op = -1;
	sembufStruct->sem_flg = semFlags;
	semop(semID, sembufStruct, 1);
}

void sem_up (int semID, unsigned short semNumber, short semFlags, struct sembuf *sembufStruct){
	sembufStruct->sem_num = semNumber;
	sembufStruct->sem_op = 1;
	sembufStruct->sem_flg = semFlags;
	semop(semID, sembufStruct, 1);
}

long get_time(){
	long timer;
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	timer = t.tv_nsec / 1000;
	return timer;
}

int main(int argc, char **argv) {
	int chairs;
	void* tmp;
	int n = 4;
	int check;
	struct sembuf sembufStruct;
	pid_t client;
	queue = NULL;

	if (argc != 2){
		printf("Proszę podać liczbę krzeseł.\n");
		exit(1);
	}
	chairs = atoi(argv[1]);
	printf("Ilość krzesel: %d\n", chairs);

	signal(SIGINT, handleSig);

	key_t key = ftok(getenv("HOME"), 'm');
	//printf("Klucz: %d\n", key);


	atexit(removing);
	shm = shmget(key, sizeof(Queue), IPC_CREAT | 0626);
	if (shm == -1)
		perror("Błąd shmget");


	tmp = shmat(shm, NULL, 0);
	if ((int)tmp == -1)
		perror("Błąd shmat");

	queue = (Queue*)tmp;
	make_queue(chairs, queue);

	//tworzenie zbioru semaforow
	sem = semget(key, n, IPC_CREAT | 0626);
	if (sem == -1)
		perror("Błąd semget");


	check = semctl(sem, 0, SETVAL, 0);		//
	if (check == -1)
		perror("Błąd sem_open");

	//ustawienie wart semafora na 1
	for (int i = 1; i < n - 1; i++){
		check = semctl(sem, i, SETVAL, 1);
		if (check == -1)
			perror("Błąd semctl");
	}
	//int semID, unsigned short semNumber, short semFlags, struct sembuf *sembufStruct
	while(1) {
		//obudzenie lub wolne krzeslo ->
		sem_down(sem, 0, 0, &sembufStruct);	//0 - semafor golibrody
		sem_down(sem, 1, 0, &sembufStruct);	//1 - semafor krzeslowy
		client = queue->chair_id;
		sem_up(sem, 1, 0, &sembufStruct);

		//-> strzyzenie
		printf("[%li] Golibroda strzyże klienta %d.\n", get_time(), client);
		kill(client, SIGRTMIN);
		printf("[%li] Zakonczenie strzyzenia klienta %d.\n", get_time(), client);


		while(1) {
			sem_down(sem, 1, 0, &sembufStruct);
			client = get(queue);
			//ktos jest w kolejce
			if (client != -1){
				sem_up(sem, 1, 0, &sembufStruct);
				printf("[%li] Golibroda strzyże klienta %d.\n", get_time(), client);
				kill(client, SIGRTMIN);
				printf("[%li] Zakonczenie strzyzenia klienta %d.\n", get_time(), client);

			}
			else {
				printf("[%li] Golibroda zasypia.\n", get_time());
				sem_down(sem, 0, 0, &sembufStruct);
				sem_up(sem, 1, 0, &sembufStruct);
				break;
			}
		}
	}
}
