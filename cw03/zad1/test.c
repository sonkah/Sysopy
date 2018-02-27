#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>



int main(int argc, char *argv[]){

	char* var;

	if (argc != 2){
		printf("Prosze podac nazwe zmiennej srodowiskowej.\n");

		exit(1);
	}

	var = argv[1];

	if (getenv(var) == NULL)

    printf("Nie ma zmiennej srodowiskowej o nazwie %s.\n", var);
	else
    printf("Wartosc zmiennej srodowiskowej %s: %s\n", var, getenv(var));

	return (0);
}
