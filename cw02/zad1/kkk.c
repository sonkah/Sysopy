#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

void print_arg_error();
void generate(char* file_name, int length, int number_of_rec);
void print_info();
int cases(char*,char*);

void sys_shuffle (char* filename, int length, int number_of_rec){

  int file = open(filename, O_RDWR);

  char *buff1=calloc(1,(size_t)length);
  char *buff2=calloc(1,(size_t)length);
  int i, j;
  long int end = lseek(file, 0, SEEK_END);
  lseek(file, 0, SEEK_SET);

  for(i = 0; i  < number_of_rec-1; i++){
    read(file, buff1,length);
    j = i + rand()%(number_of_rec-i);
    lseek(file, j *(length+1), SEEK_SET);
    read(file, buff2, length);

    lseek(file,i * (length+1), SEEK_SET);    //przesuwam offset
    write(file, buff1, length);               //wstawiam w to miejsce buff2
    lseek(file, j * (length+1), SEEK_SET);   //tak samo to
    write(file, buff2, length );
  }
  free(buff1);
  free(buff2);
  close(file);
}


void lib_shuffle(const char *filename,int length,int number_of_rec) {
  FILE* file = fopen(filename, "r+w");    //int
  if (file< 0 || fseek(file, 0, SEEK_SET) != 0){
    printf("Blad podczas otwierania pliku\n");
    exit(1);
  }


  char *buff1 = calloc(1,(size_t)length);
  char *buff2 = calloc(1,(size_t)length);

  fseek(file, 0, SEEK_END);

  long int end = ftell(file);

  int i, j;
  for (i = 0; i < length; i++) {
    fseek(file,0,SEEK_SET);
    j = i + rand()%(number_of_rec-i);
    fread(buff1, (size_t)length,1,file);
    fseek(file, j*length , SEEK_SET);
    fread(buff2, (size_t)length,1,file);
    fseek(file, j*length , SEEK_SET);


    fwrite(buff1, (size_t)length,1,file);
    fseek(file, i*length , SEEK_SET);
    fwrite(buff2, (size_t)length,1,file);
  }

  fclose(file);
  free(buff1);
  free(buff2);
}


void sys_sort(char *filename, int length, int number_of_rec){

  char* buff1 =(char*)calloc(length+1,sizeof(char));
  char* buff2 =(char*)calloc(length+1,sizeof(char));

  int file = open (filename, O_RDWR);
  if (file< 0){
    printf("Blad podczas otwierania pliku.\n");
    exit(1);
  }

  if(lseek(file, 0, SEEK_SET)!=0){
	printf("Blad podczas otwierania pliku.\n");
    exit(1);
  }

  for (int i = 0;i<= number_of_rec;i++){
    for (int j = 1; j <number_of_rec; j++){

      lseek(file, j*(length +1), SEEK_SET);

      read(file,buff1, length+1);
      read(file,buff2, length +1);

      if (strcmp(buff1, buff2) >= 0) {
        lseek(file, j*(length +1), SEEK_SET);

		write(file, buff1, length+1);
        write(file, buff2, length+1);

      }
    }
  }
  free(buff1);
  free(buff2);
  close(file);
}

void lib_sort(char *filename,int length, int number_of_rec){
  FILE* file = fopen(filename, "r+w");

  if (file == NULL || fseek(file, 0, SEEK_SET) != 0){
    printf("Blad podczas odczytu pliku/n");
    exit(1);
  }

  char *buff1=(char*)calloc(length, sizeof(char));
  char *buff2=(char*)calloc(length, sizeof(char));

  for (int i = 0; i < number_of_rec; i++) {
    fseek(file, 0, SEEK_SET);

    for (int j = 1; j < number_of_rec; j++) {
      fread(buff2, length, 1, file);
      if (strcmp(buff1, buff2) > 0) {
        fseek(file, -(length * 2), SEEK_CUR);
        fwrite(buff2, length, 1, file);
        fwrite(buff1, length, 1, file);

      }
    }
  }
  free(buff1);
  free(buff2);
  fclose(file);
}



int main(int argc, char *argv[]){

  if (argc != 6) {
    print_arg_error();
    exit(1);
  }
  char *option1 = argv[1];
  char *option2 = argv[2];
  char *filename = argv[3];
  int number_of_rec = atoi(argv[4]);
  int length = atoi(argv[5]);
  printf("%d",length);
  struct tms *start = (struct tms*)malloc(sizeof(struct tms));
  struct tms *stop = (struct tms*)malloc(sizeof(struct tms));

  switch(cases(option1,option2)){
    case 1: //generate
      times(start);
      generate(filename,length,number_of_rec);
      times(stop);
      print_info(start,stop);
      break;

    case 2: //sys sort
      printf("Sys Sort:\n");
      times(start);
      sys_sort(filename,length,number_of_rec);
      times(stop);
      print_info (start, stop);
      break;

    case 3: //lib sort
      printf("Lib Sort:\n");
      times(start);
      lib_sort(filename,length,number_of_rec);
      times(stop);
      print_info (start,stop);
      break;

    case 4: //sys shuffle
      printf("Sys Shuffle:\n");
      times(start);
      sys_shuffle(filename,length,number_of_rec);
      times(stop);
      print_info (start, stop);
      break;

    case 5: //lib shuffle
      printf("Lib Shuffle:\n");
      times(start);
      lib_shuffle(filename,length,number_of_rec);
      times(stop);
      print_info (start, stop);
      break;

    default: //niepoprawna opcja
      print_arg_error();
      break;
  }
  return 0;
}



void generate(char* filename,int length,int number_of_rec){

  FILE* randomf = fopen("/dev/urandom", "r");
  if (randomf == NULL) {
    printf("Blad podczas otwierania pliku /dev/random.\n");
    exit(1);
  }
  FILE* file = fopen(filename, "w");
  if(file ==NULL){
    printf("Blad podczas otwierania pliku do zapisu.\n");
  }

  printf("Generowanie %d rekordow o dlugosci %d.\n", number_of_rec,length);

  char* buf = malloc(sizeof(char) * length);
  char c;

  for (int i = 0;i < number_of_rec; i++) {
    for (int j = 0;j < length; j++) {

      c = (char)getc(randomf);
      buf[j] = c;
    }

    if (fwrite(buf, sizeof(char), length, file)!=length) {
      printf("Blad zapisu do pliku.\n");
      exit(1);
    }
  }
  fclose(file);
  fclose(randomf);
  free(buf);
}

void print_arg_error(){
  printf("Powinienes podac 5 argomentow:\n");
  printf("[lib\\sys] - wybor czy korzystac z funkcji wbudowanych czy bibliotecznych.\n");
  printf("[generate\\shuffle\\sort] - wybor czynnosci ktora ma wykonywac program.\n");
  printf("Nazwa pliku.\n");
  printf("Ilosc rekordow.\n");
  printf("Wielkosc 1 rekordu w bajtach.\n");
}



void print_info(struct tms* start, struct tms* end){
  printf("Czas systemowy: %ld,\nCzas uzytkownika: %ld.\n \n", end->tms_stime-start->tms_stime, end->tms_utime-start->tms_utime);
}



int cases(char *option1, char* option2){

  if(strcmp(option2,"generate")==0) return 1;

  else if(strcmp(option2,"sort")==0){
    if(strcmp(option1,"sys")==0) return 2;
    else if(strcmp(option1,"lib")==0) return 3;
  }
  else if(strcmp(option2,"shuffle")==0){
    if(strcmp(option1,"sys")==0) return 4;
    else if(strcmp(option1,"lib")==0) return 5;
  }
  else return (-1);
}
