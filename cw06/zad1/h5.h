#ifndef HELP5
#define HELP5

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define MAX_CLIENTS 3
#define MAX_LENGTH 20
#define MAX_KEY 10

//rodzaje komunikatów
static const int END = 0;
static const int CONNECT = 1;
static const int DISCONNECT = 2;
static const int ECHO = 3;
static const int LETTER = 4;
static const int TIME = 5;
static const int QFULL = 6;


typedef struct message {
  pid_t pid;
  //info_struct message info;
  short type;
  char text[MAX_LENGTH];
}message;

/* typedef struct info_struct{
   short msg_type;
   char txtmsg[MESSAGE_LENGTH];
 }*/

const size_t MESSAGE_SIZE = (sizeof(struct message) - sizeof(long)); //?????
//const int SERVER_ID = 's';
const char *PATHNAME = "HOME";


#endif
