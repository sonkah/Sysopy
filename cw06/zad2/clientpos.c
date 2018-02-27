#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <fcntl.h>
#include "hpos.h"

int main(void){
  char name[MAX_LENGTH];
  char read_buf[MAX_LENGTH];
  char write_buf[MAX_LENGTH];
  memset(read_buf, 0, MAX_LENGTH);
  memset(write_buf, 0, MAX_LENGTH);
  memset(name, '\0', MAX_LENGTH);
  pid_t pid = getpid();
  sprintf(name, "/%d", pid);

  int server_des = mq_open(QNAME, O_WRONLY);
  if (server_des == -1){
		printf("Błąd otwierania kolejki serwera.\n");
		exit(1);
	}

  int client_des;
  struct mq_attr client_q;
  client_q.mq_flags = 0;
  client_q.mq_maxmsg = 4;
  client_q.mq_msgsize = MAX_LENGTH;
  client_q.mq_curmsgs = 0;
  client_des = mq_open(name, O_CREAT | O_RDONLY, 0644, &client_q);
  if(client_des!=-1){
    printf("Kolejka klienta utworzona.\n");
  }

  sprintf(write_buf, "%d#%d#%d", CONNECT, pid, client_des);
  mq_send(server_des, write_buf, MAX_LENGTH, 0);
  printf("wyslane\n");
  mq_receive(client_des, read_buf, MAX_LENGTH, NULL);
//  printf("idk otrzymane\n");

  short types = atoi(strtok(read_buf, "#"));
  if(types==QFULL){
    printf("Kolejka pełna.\n");
  }

  size_t size;
  char* line = NULL;
  char* textmes = NULL;
  while(1){
    memset(write_buf, 0, MAX_LENGTH);
    memset(read_buf, 0, MAX_LENGTH);
    printf("\nPolecenia: \n 7.koniec\n 2.rozłącz,\n 3.echo,\n 4.duże litery na małe,\n 5.czas.\n");
    int gcheck = getline(&line, &size, stdin);
    if(gcheck != -1){

      types = atoi(line);
      if(types==ECHO || types == LETTER){
        printf("Podaj wiadomość:");
        getline(&line, &size, stdin);
      }
      if(types == END){
        printf("Koniec działania programu.\n");
        sprintf(write_buf, "%d#%d", END, pid);
        mq_send(server_des, write_buf, MAX_LENGTH, 0);
        break;
      }
      else if(types == DISCONNECT){
        printf("Rozłączanie.\n");
        sprintf(write_buf, "%d#%d", DISCONNECT, pid);
        mq_send(server_des, write_buf, MAX_LENGTH, 0);
        break;
      }
      else if(types == ECHO){
        printf("Echo\n");
        sprintf(write_buf, "%d#%d#%s",ECHO, pid, line);
        mq_send(server_des, write_buf, MAX_LENGTH, 0);
      }
      else if(types == LETTER){
        printf("zamiana duzych liter na małe\n");
        sprintf(write_buf, "%d#%d#%s",LETTER, pid, line);
        mq_send(server_des, write_buf, MAX_LENGTH, 0);
      }
      else if(types == TIME){
        printf("ktora godzina?\n");
        sprintf(write_buf, "%d#%d",TIME, pid);
        mq_send(server_des, write_buf, MAX_LENGTH, 0);
      }
      else{
        printf("Błędne polecenie");
        continue;
      }
      mq_receive(client_des, read_buf, MAX_LENGTH, NULL);
      textmes = strtok(read_buf, "#");
      textmes = strtok(NULL, "#");
      textmes = strtok(NULL, "#");
      printf("%s\n", textmes);
    }
  }
  mq_close(client_des);
	mq_unlink(name);
  return 0;
}
