#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include "h5.h"

int main(void){
  //dostep do kolejki serwera
  //stworzenie swojej kolejki
  //wysylam wiadomosci na kolejke serwera, otrzymuje do swojej

  char* path = getenv(PATHNAME);
  if(path == NULL){
    printf("nie dziala");
    exit(1);
  }

  key_t key = ftok(path, MAX_KEY);
  int server_q = msgget(key, 0644 | IPC_CREAT);
  int my_q = msgget(IPC_PRIVATE, 0644);
  if(server_q == -1 || my_q == -1){
    printf("Nie udało się uzyskać dostępu do kolejki\nsq = %d\nmyq=%d\n", server_q, my_q);
  }
  else{
    printf("Dostęp do kolejek\n" );
  }

  message msg;
  message msg2;
  //message: pid, type, text
  msg.pid = getpid();
  msg.type = CONNECT;
  sprintf(msg.text, "%d", my_q);
  int sendcheck = msgsnd(server_q, &msg, MESSAGE_SIZE, 0);
  if(sendcheck == -1){
    printf("nie udalo sie wyslać wiadomości z logowaniem do serwera\n");
  }
  int reccheck = msgrcv(my_q, &msg, MESSAGE_SIZE, 0,0);
  if(sendcheck == -1){
    printf("Błąd przy odbieraniu wiadomosci\n");
  }
  if(msg.type == CONNECT){
    printf("Połączono z sewerem.\n");
  }
  else{
    printf("o nie ;c\n");
  }
  int types;
  size_t size = 0;
  char* line = NULL;
  printf("Polecenia: \n 0.koniec\n 2.rozłącz,\n 3.echo,\n 4.wersaliki,\n 5.czas.\n");
  while(1){
    int gcheck= getline(&line, &size, stdin);
    if(gcheck!= -1){
      types = atoi(line);
      printf("typ: %d\n", types);

      if(types == ECHO || types == LETTER){
        printf("Podaj wiadomość:\n");
        getline(&line, &size, stdin);
        sprintf(msg2.text, "%s", line);
      }
      msg2.pid = getpid();
      if (types == END){
        msg2.type = END;
        msgsnd(server_q, &msg2, MESSAGE_SIZE, 0);
        break;
      }
      else if(types == DISCONNECT){
        msg2.type = DISCONNECT;
        msgsnd(server_q, &msg2, MESSAGE_SIZE, 0);
        printf("Rozłączono z serwerem.\n");
        break;
      }
      else if(types == ECHO){
        msg2.type = ECHO;
        msgsnd(server_q, &msg2, MESSAGE_SIZE, 0);
      }
      else if(types == LETTER){
        msg2.type = LETTER;
        msgsnd(server_q, &msg2, MESSAGE_SIZE, 0);
      }
      else if (types == TIME){
        msg2.type = TIME;
        msgsnd(server_q, &msg2, MESSAGE_SIZE, 0);
      }
      msgrcv(my_q, &msg2, MESSAGE_SIZE, 0, 0);
      if(types == TIME || types ==LETTER || types == ECHO){
        printf("Wiadomość zwrotna z serwera:\n");
        printf("%s\n",msg2.text);
      }
    }
  }

  int check = msgctl(my_q, IPC_RMID, NULL);
  if(check == -1){
    printf("Nie udało się usunąć kolejki.\n");
    exit(1);
  }
  else
    printf("Kolejka usunięta.\n");
}
