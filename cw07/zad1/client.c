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


Queue* queue = NULL;
int shaved = 0;
int returned;
int clients;

long get_time(){
	long timer;
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	timer = t.tv_nsec / 1000;
	return timer;
}

void decrease (int semID, unsigned short semNumber, short semFlags, struct sembuf *sembufStruct){
	sembufStruct->sem_num = semNumber;
	sembufStruct->sem_op = -1;
	sembufStruct->sem_flg = semFlags;
	semop(semID, sembufStruct, 1);
}

void increase (int semID, unsigned short semNumber, short semFlags, struct sembuf *sembufStruct){
	sembufStruct->sem_num = semNumber;
	sembufStruct->sem_op = 1;
	sembufStruct->sem_flg = semFlags;
	semop(semID, sembufStruct, 1);
}

void rem(){
	shmdt(queue);
}

void sig_handler(int sig){
	if (sig == SIGINT){
		printf("SIGINT!\n");
		exit(0);
	}
	else if (sig == SIGRTMIN){
		shaved++;
	}
}



void work (int s, struct sembuf* sembufStruct, sigset_t maska, int sem){
	int state;
	pid_t cpid;

	for (int i = 1; i <= clients; i++){
		pid_t id = fork();
		if (id < 0){
			printf("Błąd fork\n");
			exit(1);
		}
		else if (id == 0){
			while(shaved < s){
				decrease(sem, 2, 0, sembufStruct);
				decrease(sem, 1, 0, sembufStruct);
				cpid = getpid();

				state = semctl(sem, 0, GETVAL);
				if (state == -1){
					printf("Błąd semctl\n");
					exit(1);
				}
				else if(state == 0){
					increase(sem, 0, 0, sembufStruct);
					printf("[%li] Klient %d budzi golibrodę.\n", get_time(), cpid);

					increase(sem, 0, 0, sembufStruct);
					queue->chair_id = cpid;
					returned = 1;
				}
				else {
					int x = put(queue, cpid);
					if(x == -1){
						printf("[%li] Brak miejsca w poczekalni dla klienta %d.\n", get_time(), cpid);
						returned = -1;
					}
					else {
						printf("[%li] Klient %d zajmuje miejsce w poczekalni.\n", get_time(), cpid);
						returned = 0;
					}
				}

				increase(sem, 1, 0, sembufStruct);
				increase(sem, 2, 0, sembufStruct);
				if (returned != -1){
					sigsuspend(&maska);
					printf("[%li] Klient %d został ostrzyżony i wychodzi.\n", get_time(), cpid);

				}
			}
			exit(0);
		}
	}
}



int main(int argc, char **argv) {
	atexit(rem);
	struct sembuf sembufStruct;
	int s;
	key_t key;
	int shm, sem;
	void *tmp;
	sigset_t maska, maskb;
	int stop = 0;

	signal(SIGINT, sig_handler);
	signal(SIGRTMIN, sig_handler);

	if (argc != 3){
		printf("Proszę podać ilość klientów i strzyżeń.\n");
		exit(1);
	}
	clients = atoi(argv[1]);
	s = atoi(argv[2]);
	printf("Ilość klinetów: %d\n\n", clients);
	printf("Ilość strzyżen: %d\n\n", s);

	key = ftok(getenv("HOME"), 'm');

	sem = semget(key, 0, 0);
	if (sem == -1)
		perror("Błąd");

	shm = shmget(key, 0, 0);
	if (shm == -1)
		perror("Błąd");

	tmp = shmat(shm, NULL, 0);
	if ((int)tmp == -1)
		perror("Błąd");

	queue = (Queue*)tmp;

	sigfillset(&maska);
	sigdelset(&maska, SIGRTMIN);
	sigdelset(&maska, SIGINT);
	sigemptyset(&maskb);
	sigaddset(&maskb, SIGRTMIN);
	sigprocmask(SIG_BLOCK, &maskb, NULL);

	work(s, &sembufStruct, maska, sem);

	while(stop == 0){
		wait(NULL);
		if(ECHILD == errno)
			stop = 1;
	}

	printf("Wszyscy klienci zostali ostrzyżeni.\n");
	return 0;
}
