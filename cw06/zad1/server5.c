#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <time.h>
#include "h5.h"

int clients[MAX_CLIENTS];
int client_pids[MAX_CLIENTS];
int counter = 0;


void remove_queue(int queue){
  int check = msgctl(queue, IPC_RMID, NULL);
  if(check == -1){
    printf("Nie udało się usunąć kolejki.\n");
    exit(1);
  }
  else
    printf("Kolejka usunięta.\n");
}


void handle_connect(message* msg){
  if(counter == MAX_CLIENTS){
    printf("Nie można podłączyć więcej klientów.");
    msg->type = QFULL;
  }
  else{
    //sprawdzanieczy nie został wcześniej połączony!!!!!!
    printf("Podłączanie klienta o PID = %d.\n", msg->pid);
    int client_q = atoi(msg->text);
    //printf("%d\n", server_pid);
    clients[counter] = client_q;
    client_pids[counter] = msg->pid;
    sprintf(msg->text, "%d", counter);
    counter++;
    int check = msgsnd( client_q, msg, MESSAGE_SIZE, 0);
  }
}

void handle_disconnect(pid_t pid){
  printf("Rozłączanie klienta.\n");
  int index = -1;
  for(int i=0; i<counter; i++){
    if(client_pids[i] == pid){
      index = i;
    }
  }
  if(index == -1){
    printf("Klient o takim pidzie nie jest podłączony.\n" );
  }
  else{
    for(int j = index; j < counter-1; j++){
      clients[j] = clients[j+1];
      client_pids[j] = client_pids[j+1];
    }
    counter--;
    printf("Klient PID: %d rozłączony\n", pid);
  }
}

void handle_echo(message* msg){
  printf("Echo.\n");
  int client_q = -1;
  for(int i=0; i<counter; i++){
    if(client_pids[i] == msg->pid){
      client_q = clients[i];
    }
  }
  if(client_q == -1){
    printf("Klient o takim pidzie nie jest podłączony.\n");
  }
  else{
    msg->pid = getpid();
    msgsnd(client_q, msg, MESSAGE_SIZE, 0);
  }
}

void handle_letter(message* msg){
  printf("Małe litery.\n" );
  int client_q = -1;
  for(int i=0; i<counter; i++){
    if(client_pids[i] == msg->pid){
      client_q = clients[i];
    }
  }
  for (int i = 0 ; i < MAX_LENGTH || msg->text[i]!='\0'; i++){
    if(msg->text[i]<95&&msg->text[i]>62){
      msg->text[i]=(msg->text[i]+32);
  //    printf("%c\n", msg->text[i]);
    }
  }
  if(client_q == -1){
    printf("Klient o takim pidzie nie jest podłączony.\n");
  }
  else{
    //msg->text = mtext;
    msg->pid = getpid();
    msgsnd(client_q, msg, MESSAGE_SIZE, 0);
  }
}

void handle_time(message* msg){
  printf("Godzina.\n");
  int client_q = -1;
  for(int i=0; i<counter; i++){
    if(client_pids[i] == msg->pid){
      client_q = clients[i];
    }
  }
  if(client_q == -1){
    printf("Klient o takim pidzie nie jest podłączony.\n");
  }
    time_t t;
    struct tm * ptr;
    time( &t);
    ptr = localtime(&t);
    sprintf(msg->text, "godz:%d:%d:%d", ptr->tm_hour, ptr->tm_min, ptr->tm_sec);
    msgsnd(client_q, msg, MESSAGE_SIZE, 0);
}

int main(void){
  char* path = getenv(PATHNAME);
  if(path == NULL){
    printf("nie dziala");
    exit(1);
  }

  key_t key = ftok(path, MAX_KEY);
  int sq = msgget(key, 0644 | IPC_CREAT); //server queue
  message msg;

  int mes_flag;
  while(1){
    mes_flag = msgrcv(sq, &msg, MESSAGE_SIZE, 0, 0);
    printf("Wiadomość od %d.\n", msg.pid);
    if (mes_flag == -1)
			continue;
		else{
      if(msg.type == CONNECT){
        handle_connect(&msg);
      }
      else if(msg.type == DISCONNECT){
        handle_disconnect(msg.pid);
      }
      else if(msg.type == ECHO){
        handle_echo(&msg);
      }
      else if(msg.type == LETTER){
        handle_letter(&msg);
      }
      else if(msg.type == TIME){
        handle_time(&msg);
      }
      else if(msg.type == END){
        break;
      }
      else{
        printf("Błędne polecenie");
      }
    }
  }
  remove_queue(sq);

  return 0;
}
