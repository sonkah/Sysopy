#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ftw.h>
#include <dirent.h>
#include <sys/stat.h>

int size;
void print_files_details1(char*path, int size);

//int fd(const char *fpath, const struct stat *stat_structure, int tflag, struct FTW *ftwb);

typedef struct FTW FTW;

void nftw(char *path, int (*fd)(const char *, const struct stat *, int, struct FTW *), int i, int flag);


int fd(const char *fpath, const struct stat *stat_structure, int tflag, struct FTW *ftwb) {

  if (tflag == FTW_F ) {

    char p[PATH_MAX+1];
    char *actual_path;

    FILE *file = fopen(fpath, "r");
    fseek(file, 0, SEEK_END);

    long int size_f = ftell(file);
    if(size_f <= size ){
      actual_path = realpath(fpath, p);
      fclose(file);
      printf("\n" );

      printf("Sciezka bezwzgledna: %s\nRozmiar pliku: %ld B\nCzas od modyfikacji: %ld\n",actual_path, size_f, stat_structure->st_mtime);


      printf("Uprawnienia pliku: \t");

      printf("-");
      if(stat_structure->st_mode & S_IRUSR) printf("r");
      else printf("-");

      if(stat_structure->st_mode & S_IWUSR) printf("w");
      else printf("-");

      if(stat_structure->st_mode & S_IXUSR) printf("x");
      else printf("-");

      if(stat_structure->st_mode & S_IRGRP) printf("r");
      else printf("-");

      if(stat_structure->st_mode & S_IWGRP) printf("w");
      else printf("-");

      if(stat_structure->st_mode & S_IXGRP) printf("x");
      else printf("-");

      if(stat_structure->st_mode & S_IROTH) printf("r");
      else printf("-");

      if(stat_structure->st_mode & S_IWOTH) printf("w");
      else printf("-");

      if(stat_structure->st_mode & S_IXOTH) printf("x");
      else printf("-");

      printf("\n" );
    }
  }
  return 0;
}


int main(int argc, char *argv[]) {

  if(argc !=3) {
    printf("Program wymaga podania 2 argumentow:\n - sciezki do katalogu\n - rozmiaru pliku\n");
    exit(1);
  }

  size = atoi(argv[2]);    //rozmiar w bajtach
  char *pathname = argv[1];         //sciezka wzgledna lub bezwzgl do katalogu

  if(size<= 0) {
    printf("Rozmiar pliku musi byc wiekszy od 0.");
    exit(1);
  }

  int method;
  printf("Ktorej metody chcesz uzyc:\n");
  scanf("%d", &method);

  if(method==1)
    print_files_details1(pathname, size);
  else if(method ==2)
   nftw(pathname, fd, 15, FTW_F);
  else{
    printf("Mozesz wybrac tylko:\n1 - wykonanie za pomoca opendir, readdir itd.\n2 - wykonanie za pomoca fcntl\n");
    exit(1);
  }
  return 0;
}


// opendir, readdir
void print_files_details1 (char *path, int size) {

  DIR *dir = opendir(path);

  struct stat stat_structure; //deklaracja stat

  if(lstat(path,&stat_structure)<0){  //lstat nie podaza za dowiazaniami symbolicznymi
    printf("Blad przy przypisywaniu struktury stat.\n Najprawdopodobniej sciezka jest bledna.\n");
  };

  int len = strlen(path);  //dlugosc sciezki

  char filename[FILENAME_MAX];

  strcpy(filename, path);  //

  if(filename[len-1]!='/'){
    filename[len] = '/'; //dodaje na koniec /
    len++;      //dodaje 1 na znak konca linii
  }

  char *actualpath;
  char sth[PATH_MAX+1];
  struct dirent *ent;

  while ((ent = readdir(dir)) != NULL) { //wszytuje wskaznik do elementu katalogu

    if (strcmp(ent->d_name, "..")!=0 && strcmp(ent->d_name, ".")!=0){

      strncpy(filename + len, ent->d_name, FILENAME_MAX - len);
  //  printf("\n%s\n",filename);
      lstat(filename, &stat_structure);

      if (S_ISDIR(stat_structure.st_mode)) {     //sprawdzamy czy jest katalogiem
        print_files_details1(filename,size);  // jak jest katalogiem to rekurencja
      }

      else if (S_ISREG(stat_structure.st_mode) == 1) {  //jak plik normalny

        FILE *file = fopen(filename,"r");  //Otwiera plik do odczytu

        fseek(file,0,SEEK_END);    //

        long size_f = ftell(file);      //

        if(size_f<=size ){
          actualpath = realpath(filename, sth);
          fclose(file);
          printf("\n" );
          printf("Sciezka bezwzgledna: %s\nRozmiar pliku: %ld B\nCzas od modyfikacji: %ld\n",actualpath, size_f, stat_structure.st_mtime);
          printf("Uprawnienia:  ");
          printf(" -");
          if(stat_structure.st_mode & S_IRUSR) printf("r");
          else printf("-");

          if(stat_structure.st_mode & S_IWUSR) printf("w");
          else printf("-");

          if(stat_structure.st_mode & S_IXUSR) printf("x");
          else printf("-");

          if(stat_structure.st_mode & S_IRGRP) printf("r");
          else printf("-");

          if(stat_structure.st_mode & S_IWGRP) printf("w");
          else printf("-");

          if(stat_structure.st_mode & S_IXGRP) printf("x");
          else printf("-");

          if(stat_structure.st_mode & S_IROTH) printf("r");
          else printf("-");

          if (stat_structure.st_mode & S_IWOTH) printf("w");
          else printf("-");

          if(stat_structure.st_mode & S_IXOTH) printf("x");
          else printf("-");

          printf("\n" );
        }
      }
    }
  }
  closedir(dir); //zamykamy
}


//nftw
