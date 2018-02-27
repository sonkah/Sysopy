#ifndef HELPPOS
#define HELPPOS

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <mqueue.h>

#define MAX_CLIENTS 3
#define MAX_LENGTH 50
#define MAX_KEY 10

//rodzaje komunikatów
static const int END = 7;
static const int CONNECT = 1;
static const int DISCONNECT = 2;
static const int ECHO = 3;
static const int LETTER = 4;
static const int TIME = 5;
static const int QFULL = 6;

typedef struct message {
  pid_t pid;
  short type;
  char text[MAX_LENGTH];
}message;

const size_t MESSAGE_SIZE = (sizeof(struct message) - sizeof(int)); //?????
const int SERVER_ID = 's';
const char* PATHNAME = "HOME";
const char* QNAME = "/QUEUE1";


#endif
