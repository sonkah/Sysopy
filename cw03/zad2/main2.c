#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#define MAX_ARG_NUM 10//MAX_ARG_NUM
#define MAX_ARG_LENGTH 40//MAX_ARG_LENGTH
#define BUF_LENGTH 150// BUF_LENGTH = 128;


int skipspaces(int i, char* line)
{
  while(line[i]==' '&& i<BUF_LENGTH-1){
    i++;
  }
  return i;
}

int main(int argc, char* argv[]){
  float time2 = 0;
	float time1 = 0;
  if (argc != 4){
    printf("NProsze podac sciezke do pliku, czas w s oraz rozmiar w MB.\n");
    exit(1);
  }
  int cputime = atoi(argv[2]);
	int vmemory = atoi(argv[3]);
  if(cputime <0 || vmemory < 0){
    printf("Czas i pamiec nie moga byc mniejsze od 0.\n");
    exit(1);
  }
  vmemory *= (1024 * 1024);

	FILE* file;
  char* name = argv[1];
  file = fopen(name,"r");

	if (file == NULL){
		printf("Blad przy otwieraniu pliku.\n");
		exit(1);
	}

  //pid_t pid = getpid();

	char* line = malloc(sizeof(char)*BUF_LENGTH);
	char* value = malloc(sizeof(char)*BUF_LENGTH);     //wartosc zmiennej
  char* vname = malloc(sizeof(char)*BUF_LENGTH);     //nazwa zmiennej

  int linenum = 0;
  while ((line = fgets(line, BUF_LENGTH, file)) != NULL) {
    linenum++;
    if (line[0] == '\n') continue;
    if (line[0]=='#'){
      int i = 1;
      int j=0;
      //i = skipspaces(i, line);
      //przepisuje nazwe zmiennej do nastepnej spacji/entera/konca linii
      while(line[i]!=' ' && i<BUF_LENGTH && line[i]!='\0' && line[i]!='\n'){
        vname[j] = line[i];
        i++;
        j++;
      }
      //pomijanie spacji po nazwie.
      i = skipspaces(i, line);
      j = 0;
      //pętla przepisywania wartosci.
      //nie  wywola sie jesli nie podano wartosci
      while(line[i]!= ' ' && line[i]!= '\n' && line[i]!='\0'){
        value[j] = line[i];
        i++;
        j++;
      }

      int success = -123;
      //poprzednia petla sie nie wykonala wiec j dalej rowne 0
      if(j==0){
        //unsetenc 0 sukces, -1 err
        success= unsetenv(vname);
        if(success==0)
          printf("Usunieto zmienna %s.\n", vname);
        else{
          printf("Nie udalo sie usunac zmiennej %s.\n", vname);
          exit(1);
        }
      }
      else{
        success = setenv(vname, value, 1);
        if(success==0)
          printf("Dodano zmienna lub zmieniono wartosc zmiennej %s = %s.\n", vname, value);
        else{
          printf("Nie udalo sie dodac ani zmienic wartosci zmiennej srodowiskowej %s.\n", vname);
          exit(1);
        }
      }
    //  printf("\n %d /n", success);
    }

    else{
//tablica zaweierajaca max 9 arg
      char **argtab= malloc(sizeof(char*)*MAX_ARG_NUM);
      char *operation = malloc (sizeof (char)*MAX_ARG_LENGTH);

      struct rlimit * rlim;
      struct rusage rustr;
      int is_limit_set;

      int counter= 0;                              //ilosc argomentow
      int j = 0;                              //
      int i=0;                        // zaznacza miejsce w line

      i = skipspaces(i, line);
      while(i < BUF_LENGTH && line[i]!= '\0' && line[i]!=' ' && line[i]!='\n'){
        operation[j]=line[i];     //pierwsza rzecz to nazwa programu
        i++;
        j++;
      }
      i=0;
      //czytanie calej linii
      while(i < BUF_LENGTH && line[i]!='\0' && line[i]!='\n'){
        j=0;
        i = skipspaces(i, line);          //pomijamy spacje miedzy arg
        argtab[counter] = malloc(sizeof(char)*MAX_ARG_LENGTH);
        //zapisywanie 1 argumentu do tablicy
        while(line[i]!=' ' && line[i]!='\n' && line[i]!='\0'){
          argtab[counter][j] = line[i];
          i++;
          j++;
        }
        //znak konca linii po ostatnim znaku
        argtab[counter][j] = '\0';
        counter++;
      }
      //puste pola tablicy na null
      for(int i = counter; i<MAX_ARG_NUM; i++){
        argtab[i] = NULL;
      }

      int waiting;
      pid_t pid = fork();
      //proces potomny
      if(pid==0){
      	//limity////////////////////////////////////////////////
        rlim = malloc( sizeof(struct rlimit));
				rlim -> rlim_max = (unsigned) vmemory;
				rlim -> rlim_cur = (unsigned) vmemory;

				is_limit_set = setrlimit(RLIMIT_AS, rlim);

        if(is_limit_set!=0){
          printf("Blad ustawiania limitu na pamiec.\n");
          exit(1);
        }
				free(rlim);

        rlim = malloc(sizeof(struct rlimit));
				rlim -> rlim_max = (unsigned) cputime;
				rlim -> rlim_cur = (unsigned) cputime;
				is_limit_set = setrlimit(RLIMIT_CPU, rlim);

				if (is_limit_set != 0) {
					printf("Blad ustawiania limitu na czas procesora.\n");
					exit(1);
				}

        free(rlim);

        execvp(operation, argtab);

      }
      //proces macierzysty
      else if(pid > 0){
        //czeka na wykonanie procesu potomnego
        wait(&waiting);
        //jezeli proces zakonczonu poprawnie

        getrusage(RUSAGE_CHILDREN, &rustr);

				printf("\n%s:\nPamiec: %dMB\nCzas systemowy: %fs\nCzas uzytkownika: %fs\n", name, (int)rustr.ru_maxrss/1024,
                          (float) rustr.ru_stime.tv_sec-time1,
                          (float) rustr.ru_utime.tv_sec-time2);

				time1 = (float) rustr.ru_stime.tv_sec;
				time2 = (float) rustr.ru_utime.tv_sec;

        if(WIFEXITED(waiting)) {
					printf("Proces potomny zakonczony sukcesem, status %d\n", WEXITSTATUS(waiting));
				}
        else {
					printf("Proces potomny zakonczony porazka, status: %d\n", WEXITSTATUS(waiting));
        //  printf("Problem znajduje sie w  %s w linii %d.\n",operation, linenum);
				}
      }
      else{
        printf("Wystapil blad podczas tworzenia procesu.\n");
        printf("Problem sprawia polecenie %s w linii %d.\n",operation, linenum);
        exit(1);
      }

      free(operation);
      for(int i=0;i<MAX_ARG_NUM;i++) {
        free(argtab[i]);
      }
			free(argtab);

    }
    for(int licz=0;licz < BUF_LENGTH; ++licz){
       vname[licz] = '\0';
       value[licz] = '\0';
    }
  }

  free(line);
  free(value);
  free(vname);

  fclose(file);

  return 0;
}
