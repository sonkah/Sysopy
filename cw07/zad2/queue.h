#include <signal.h>
#include <stdio.h>

#define N 600

typedef struct Queue {
	int head, tail;
	int max_chair_number;
	int chair_pointer;
	pid_t chair_id;
	pid_t pid_tab[N];
} Queue;

void make_queue(int, Queue* );
int put(Queue*, pid_t);
pid_t get(Queue*);
