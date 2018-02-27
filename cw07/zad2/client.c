#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include "queue.h"

Queue* fifo;
sem_t *barber_sem;
sem_t *chairs_sem;
sem_t * clients_sem;
int shaved = 0;


void removing(){
	sem_close(clients_sem);
	sem_close(chairs_sem);
	sem_close(barber_sem);
	munmap(fifo, sizeof(Queue));
}

long get_time(){
	long timer;
	struct timespec ti;
	clock_gettime(CLOCK_MONOTONIC, &ti);
	timer = ti.tv_nsec / 1000;
	return timer;
}

void sig_handler(int s){
	if (s == SIGRTMIN)
		shaved++;
	else if (s == SIGINT){
			printf("SIGINT!\n");
			exit(0);
		}
}


int main(int argc, char **argv) {
	int shm;
	void *tmp;
	sigset_t maska, maskb;
	int s, powrot;

	if (argc != 3){
		printf("Proszę podać liczbę klientów i strzyżen.\n");
		exit(1);
	}
	int clients_num = atoi(argv[1]);
	int cuts = atoi(argv[2]);
	printf("Ilość klinetów: %d\n", clients_num);
	printf("Ilość strzyen: %d\n", cuts);

	atexit(removing);

	signal(SIGRTMIN, sig_handler);
	signal(SIGINT, sig_handler);

	shm = shm_open("/shm", O_RDWR, 0626);
	if (shm == -1)
		perror("Błąd shm_open");

	tmp = mmap(NULL, sizeof(Queue), PROT_WRITE | PROT_READ, MAP_SHARED, shm, 0);
	fifo = (Queue*) tmp;

	clients_sem = sem_open("/clients_sem", O_RDWR);
	if (clients_sem == SEM_FAILED)
		perror("Błąd sem_open");

	chairs_sem = sem_open("/chairs_sem", O_RDWR);
	if (chairs_sem == SEM_FAILED)
		perror("Błąd sem_open");

	barber_sem = sem_open("/barber_sem", O_RDWR);
	if (barber_sem == SEM_FAILED)
		perror("Błąd sem_open");

	sigfillset(&maska);
	sigdelset(&maska, SIGRTMIN);
	sigdelset(&maska, SIGINT);
	sigemptyset(&maskb);
	sigaddset(&maskb, SIGRTMIN);
	sigprocmask(SIG_BLOCK, &maskb, NULL);

	pid_t cpid;
	for (int i = 1; i <= clients_num; i++){
		cpid = fork();
		if (cpid == -1){
			printf("Błąd fork\n");
			exit(1);
		}
		else if (cpid == 0){
			while (shaved < cuts){
				if (sem_wait(clients_sem) == -1)
					perror("Błąd sem_wait");
				if (sem_wait(chairs_sem) == -1)
					perror("Błąd sem_wait");
				sem_getvalue(barber_sem, &s);
				cpid = getpid();
				if (s != 0){
					int is_place = put(fifo, cpid);
					if(is_place == -1){
						printf("[%li] Brak miejsca w poczekalni dla klienta %d.\n", get_time(), cpid);
						powrot = -1;
					}
					else {
						printf("[%li] Klient %d zajmuje miejsce w poczekalni.\n", get_time(), cpid);
						powrot = 0;
					}
				}
				else {
					if (sem_post(barber_sem) == -1)
						perror("Błąd sem_post");
					printf("[%li] Klient %d budzi golibrodę.\n", get_time(), cpid);
					if (sem_post(barber_sem) == -1)
						perror("Błąd sem_post");
					fifo->chair_id = cpid;
					powrot = 1;
				}
				if (sem_post(chairs_sem) == -1)
					perror("Błąd sem_post");
				if (sem_post(clients_sem) == -1)
					perror("Błąd sem_post");
				if (powrot != -1){
					sigsuspend(&maska);
					signal(SIGRTMIN, sig_handler);
					signal(SIGINT, sig_handler);
					printf("[%li] Klient %d został ostrzyżony i wychodzi.\n", get_time(), cpid);
				}
			}
			exit(0);
		}
	}


	while(1){
		wait(NULL);
		if(errno == ECHILD)
			break;
	}
	printf("Wszyscy klienci zostali ostrzyżeni.\n");
	return 0;
}
