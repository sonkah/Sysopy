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

sem_t *barber_sem;
sem_t *chairs_sem;
sem_t * clients_sem;
Queue* fifo;

void removing(){
	shm_unlink("/shm");
	munmap(fifo, sizeof(Queue));
	sem_close(chairs_sem);
	sem_unlink("/chairs_sem");
	sem_close(clients_sem);
	sem_unlink("/clients_sem");
	sem_close(barber_sem);
	sem_unlink("/barber_sem");
}

void sig_handler(int s){
	printf("SIGINT!\n");
	exit(0);
}

long get_time(){
	long timer;
	struct timespec ti;
	clock_gettime(CLOCK_MONOTONIC, &ti);
	timer = ti.tv_nsec / 1000;
	return timer;
}

int main(int argc, char **argv) {
	int chairs_num;
	int shm;
	void *tmp;

	if (argc != 2){
		printf("Proszę podać liczbę krzeseł.\n");
		exit(1);
	}
	chairs_num = atoi(argv[1]);
	printf("Ilość krzeseł na poczekalni: %d\n", chairs_num);

	atexit(removing);
	signal(SIGINT, sig_handler);


	shm = shm_open("/shm", O_CREAT | O_TRUNC | O_RDWR, 0626);
	if (shm == -1)
		perror("Błąd shm_open");

	ftruncate(shm, sizeof(Queue));
	tmp = mmap(NULL, sizeof(Queue), PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
	fifo = (Queue*) tmp;
	make_queue(chairs_num, fifo);

	clients_sem = sem_open("/clients_sem", O_CREAT | O_EXCL | O_RDWR, 0626, 1);
	if (clients_sem == SEM_FAILED)
		perror("Błąd sem_open");

	chairs_sem = sem_open("/chairs_sem", O_CREAT | O_EXCL | O_RDWR, 0626, 1);
	if (chairs_sem == SEM_FAILED)
		perror("Błąd sem_open");

	barber_sem = sem_open("/barber_sem", O_CREAT | O_EXCL | O_RDWR, 0626, 0);
	if (barber_sem == SEM_FAILED)
		perror("Błąd sem_open");

	while(1){
		if ((sem_wait(barber_sem)) == -1)
					perror("Błąd sem_wait");

		if ((sem_wait(chairs_sem)) == -1)
					perror("Błąd sem_wait");

		pid_t cpid = fifo->chair_id;
		printf("[%li] Klient %d budzi golibrodę.\n", get_time(), cpid);
		sem_post(chairs_sem);

		//strzyzenie:
		printf("[%li] Golibroda strzyże klienta %d.\n", get_time(), cpid);
		kill(cpid, SIGRTMIN);
		printf("[%li] Zakonczenie strzyzenia klienta %d.\n", get_time(), cpid);

		while(1){
			if ((sem_wait(chairs_sem)) == -1)
					perror("Błąd sem_wait");


			cpid = get(fifo);
			if (cpid == -1){
				printf("[%li] Golibroda zasypia.\n", get_time());
				if ((sem_wait(barber_sem)) == -1)
					perror("Błąd sem_wait");

				if ((sem_post(chairs_sem)) == -1)
					perror("Błąd sem_wait");
				break;
			}
			else {
				if ((sem_post(chairs_sem)) == -1)
					perror("Błąd sem_post");

				//strzyzenie:
				printf("[%li] Golibroda strzyże klienta %d.\n", get_time(), cpid);
				kill(cpid, SIGRTMIN);
				printf("[%li] Zakonczenie strzyzenia klienta: %d.\n", get_time(), cpid);
			}
		}
	}
}
