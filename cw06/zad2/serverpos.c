#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <fcntl.h>
#include <time.h>
#include "hpos.h"

int clients[MAX_CLIENTS];
int client_pids[MAX_CLIENTS];
int counter = 0;

void handle_connect(char* msg){
  printf("łączenie %s\n", msg);
  char* t = strtok(msg, "#\n\t\0"); //type
//  printf("a%s\n", t);

  t = strtok(NULL, "#\n\t\0");
  pid_t client_pid = atoi(t); //pid
//  printf("%d\n", client_pid);
  t = strtok(NULL, "#\n\t\0"); //deskryptor
  int client_q = atoi(t);
  char l[MAX_LENGTH];
  pid_t pid = getpid();
  if(counter == MAX_CLIENTS){
    printf("Nie można podłączyć więcej klientów.");
    sprintf(l, "%d#%d", QFULL, pid);
  }
  else{
    //sprawdzanieczy nie został wcześniej połączony!!!!!!

    printf("Podłączanie klienta o PID = %d.\n", client_pid);

    //printf("%d\n", server_pid);
    client_pids[counter] = client_pid;
    sprintf(l, "%d#%d#%d", CONNECT, pid, counter);
    counter++;
  }
  char name[MAX_LENGTH];
  memset(name, '\0', MAX_LENGTH);
  sprintf(name, "/%d", client_pid);
  printf("%s\n", name);
  int x = mq_open(name, O_WRONLY);
  mq_send(x, l, MAX_LENGTH, 0);
  mq_close(x);
  printf("Połączono\n");
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
      client_pids[j] = client_pids[j+1];
    }
    counter--;
    printf("Klient PID: %d rozłączony\n", pid);
  }
}

void handle_echo(char* msg){
  printf("Echo.\n");
  int client_q;
  char* t = strtok(msg, "#"); //typ
  int pid = atoi(strtok(NULL, "#")); //pid KLINTA
  t = strtok(NULL, "#");
  char l[MAX_LENGTH];
  for(int i=0; i<counter; i++){
    if(client_pids[i] == pid){
      client_q = 1;
    }
  }
  if(client_q == -1){
    printf("Klient o takim pidzie nie jest podłączony.\n");
  }
  else{
    sprintf(l, "%d#%d#%s", TIME, pid, t);
    char name[MAX_LENGTH];
    memset(name, '\0', MAX_LENGTH);
    sprintf(name, "/%d", pid);
    int x = mq_open(name, O_WRONLY);
    mq_send(x, l, MAX_LENGTH, 0);
    mq_close(x);
  }
}

void handle_letter(char* msg){
  printf("Małe litery.\n" );
  int client_q = -1;
  char* t = strtok(msg, "#");
  int pid = atoi(strtok(NULL, "#"));
  t = strtok(NULL, "#");
//  printf("a%sa\n", t);
  char l[MAX_LENGTH];
  for(int i=0; i<counter; i++){
    if(client_pids[i] == pid){
      client_q = 1;
    }
  }
  for (int i = 0 ; i < MAX_LENGTH || t[i]!='\0'; i++){
    if(t[i]<95&& t[i]>62){
      t[i]=t[i]+32;
  //    printf("%c\n", msg->text[i]);
    }
  }
  if(client_q == -1){
    printf("Klient o takim pid nie jest podłączony.\n");
  }
  else{
    char name[MAX_LENGTH];
    memset(name, '\0', MAX_LENGTH);
    sprintf(l, "%d#%d#%s", LETTER, pid, t);
    sprintf(name, "/%d", pid);
    int x = mq_open(name, O_WRONLY);
    mq_send(x, l, MAX_LENGTH, 0);
    mq_close(x);
  }
}

void handle_time(char* msg){
  printf("Godzina.\n");
  int client_q = -1;
  char* t = strtok(msg, "#");
  int pid = atoi(strtok(NULL, "#"));

  for(int i=0; i<counter; i++){
    if(client_pids[i] == pid){
      client_q = 1;
    }
  }
  char l[MAX_LENGTH];
    time_t ti;
    struct tm * ptr;
    time( &ti);
    ptr = localtime(&ti);

    sprintf(l, "%d#%d#godz:%d:%d:%d",TIME,pid, ptr->tm_hour, ptr->tm_min, ptr->tm_sec);
    char name[MAX_LENGTH];
    memset(name, '\0', MAX_LENGTH);
    sprintf(name, "/%d", pid);
    int x = mq_open(name, O_WRONLY);
    mq_send(x, l, MAX_LENGTH, 0);
    mq_close(x);
}

int main(void){

  int server_des;
  struct mq_attr server_q;
  server_q.mq_flags = 0;
	server_q.mq_maxmsg = 10;
  server_q.mq_msgsize = MAX_LENGTH;
  server_q.mq_curmsgs = 0;

  server_des = mq_open(QNAME, O_CREAT | O_RDONLY, 0644, &server_q);
  if(server_des!=-1){
    printf("kolejka utworzona\n");
  }
  else{
    printf("BŁĄD\n" );
    exit(1);
  }
  char buf[MAX_LENGTH];
  char buf2[MAX_LENGTH];
  memset(buf, 0, MAX_LENGTH);

  short types;
  while(1){
    mq_receive(server_des, buf, MAX_LENGTH, NULL);
  //  printf("%s\n", buf);
    for(int i=0; i<MAX_LENGTH; i++){
      buf2[i] = buf[i];
    }
    char* pom = strtok(buf, "#");
    types = atoi(pom);
    pom = strtok(NULL, "#");
    printf("Wiadomość od klienta: %s\n", pom);
  //  printf("aa%s %s\n", buf, buf2);
    if(types == CONNECT){
      handle_connect(buf2);
    }
    if(types == END){
      printf("Koniec.\n");
      break;
    }
    else if(types == DISCONNECT){
      pid_t p = atoi(strtok(NULL, "#"));
      handle_disconnect(p);
    }
    else if(types == ECHO){
      handle_echo(buf2);
    }
    else if(types == LETTER){
      handle_letter(buf2);
    }
    else if(types == TIME){
      handle_time(buf2);
    }
  }
  mq_close(server_des);
	mq_unlink(QNAME);
  return 0;
}
