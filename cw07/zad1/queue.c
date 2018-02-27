#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "queue.h"




void make_queue(int numberOfChairs, Queue *fifo){
	fifo->max_chair_number = numberOfChairs;
	fifo->head = 0;
	fifo->tail = 0;
	fifo->chair_pointer = 0;
	fifo->chair_id = 0;
}




int put(Queue *fifo, pid_t chair_id){
	if(fifo->chair_pointer <= fifo->max_chair_number){
		fifo->chair_pointer++;
		fifo->pid_tab[fifo->tail] = chair_id;

		if(fifo->tail + 1 >= N)
			fifo->tail = 0;
		else
			fifo->tail++;

		return 0;
	}
	return -1;
}




pid_t get(Queue *fifo){
	if(fifo->chair_pointer != 0){
		fifo->chair_pointer--;

		pid_t pid = fifo->pid_tab[fifo->head];
		if(fifo->head + 1 >= N)
			fifo->head = 0;
		else
			fifo->head++;

		return pid;
	}
	return -1;
}
