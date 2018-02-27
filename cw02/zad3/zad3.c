#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

//fcntl - manipulate file descriptor
void print_options();
void set_readlock(int file);
void set_writelock(int file);
void show_locks(int file);
void remove_lock(int file);
void read_byte(int file);
void write_to_file(int file);

int main(int argc, char **argv){

	char tmp;
	if(argc !=2){
		printf("Podano zla ilosc argumentow.\n");
		exit(1);
	}

  char * path = argv[1];
	int file = open(path, O_RDWR);
  if(file < 0){
    printf("Nie udalo sie otworzyc pliku.\n");
  }
  int option;
	while(1){
		print_options();
		scanf("%d",&option);
    if(option ==1)set_readlock(file);
    else if(option ==2) set_writelock(file);
    else if(option ==3) show_locks(file);
    else if(option ==4) remove_lock(file);
    else if(option ==5) read_byte(file);
    else if(option ==6) write_to_file(file);
    else if(option ==0){
      close(file);
      return 0;
    }
    else{ printf("%s\n", "Zla opcja.");
		}
	}
}

void set_readlock(int file){
  int byte;
  printf("\nPodaj pozycje do zaryglowania\n");
  scanf("%d",&byte);
  int waiting;
  printf("\nCzy w razie niepowodzenia chcesz czekać?:\n1 - Tak,\n0 - Nie.\n");
  scanf("%d",&waiting);

	struct flock * flck = malloc(sizeof(struct flock));

	flck->l_type = F_RDLCK;
	flck->l_whence = SEEK_SET;
	flck->l_len = 1;
	flck->l_start=byte;
  //flck->l_pid = getpid();
  if(!waiting){
      if(fcntl(file, F_SETLK, flck) <0){
        perror(NULL);
          return;
      }
  } else {
      if(fcntl(file, F_SETLKW, flck) <0){
        perror(NULL);
          return;
      }
  }
  free(flck);
}

void set_writelock(int file){
  int byte;
  printf("\nPodaj numer bajtu do zaryglowania:\n");
  scanf("%d",&byte);

  int waiting;
  printf("\nCzy w razie niepowodzenia chcesz czekać?:\n1 - Tak,\n0 - Nie.\n");
  scanf("%d",&waiting);

  struct flock *flck = calloc(1,sizeof(struct flock));

  flck->l_type = F_WRLCK;

  flck->l_whence = SEEK_SET;
  flck->l_len = 1;
	flck->l_start=byte;

 if(!waiting){
      if(fcntl(file, F_SETLK, flck) <0){
        perror(NULL);
          return;
      }
  } else {
      if(fcntl(file, F_SETLKW, flck) <0){
        perror(NULL);
          return;
      }
  }
  free(flck);
}


void show_locks(int file){

  long int endfile =lseek(file,0,SEEK_END);
	printf("plik ma dlugosc: %ld", endfile);

  int i=0;

  printf("\nZablokowane bajty:\n");

	while(i<endfile){

		struct flock *flck = malloc(sizeof(struct flock));
		flck->l_type=F_WRLCK;
    flck->l_whence=SEEK_SET;
    flck->l_start=i;
    flck->l_len=1;

    fcntl(file,F_GETLK,flck);
		if(flck->l_type == F_WRLCK){
			printf("Rygiel na zapis nr %d: %d\n",i,flck->l_pid);
		}
		else if(flck->l_type == F_RDLCK){
			printf("Rygiel na odczyt nr %d: %d\n",i,flck->l_pid);
		}

    free(flck);
		i++;
  //}
}
}

void remove_lock(int file){

  int byte;
  printf("Podaj numer bajtu do odblokowania:\n");
  scanf("%d",&byte);

  struct flock * flck = malloc(sizeof(struct flock));

  flck->l_type=F_UNLCK;
  flck ->l_len=1;
  flck ->l_whence=SEEK_SET;
  flck ->l_start=byte;

  if(fcntl(file,F_SETLK,flck)<0)
    printf("Wystapil blad.\n");
  free(flck);

}

void read_byte(int file){
  int byte;
  printf("Podaj numer bajtu w pliku:\n");

  scanf("%d",&byte);
  char letter;
  lseek(file,byte,SEEK_SET);

  read(file, &letter, 1);

  printf("%c\n",letter);
}

void write_to_file(int file){
    int byte;
		char letter;

    printf("Podaj numer bajtu w pliku i znak, ktory chcesz zapisac:\n");
    scanf("%d %c",&byte,  &letter);
    //printf("Podaj znak, ktory chcesz zapisac:");
	//	flush(stdin);
  	//scanf("%c", &letter);

    lseek(file, byte, SEEK_SET);
    if(write(file, &letter, 1)<0){
      printf("Blad zapisu.\n");
    }
    else printf("Zapis zakonczony pomyslnie.\n");
  }

void print_options(){
  printf("\n");
	printf(" Co chcesz zrobic?:\n");
	printf("  1. Ustawienie blokady odczytu na wybrany znak pliku.\n");
	printf("  2. Ustawienie blokady zapisu na wybrany znak pliku.\n");
	printf("  3. Wyswietlenie listy zaryglowanych znakow.\n");
	printf("  4. Zwolnienie wybranego rygla.\n");
	printf("  5. Odczyt wybranego znaku.\n");
	printf("  6. Zmiana wybranego znaku.\n");
	printf("  0. Koniec.\n"); //
  //printf(" \n");
}
