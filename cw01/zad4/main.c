#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dlfcn.h>
#include "address_book.h"
#include "generator.h"
#include <dlfcn.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <unistd.h>
#include <inttypes.h>


#ifndef DLL
#include"address_book.h"
#endif

int main(){

    #ifdef DLL
    void *handle = dlopen("./libaddress_book.so", RTLD_LAZY);
    Tree*(*create_tree)(void)= dlsym(handle, "create_tree");
    void (*remove_tree)(Tree*) = dlsym(handle, "remove_tree");
    void (*add_to_tree)(Tree*, TNode*)= dlsym(handle, "add_to_tree");
    TNode* (*remove_from_tree)(Tree*, TNode*)= dlsym(handle, "remove_from_tree");
    TNode* (*find_tnode_by_name)(Tree*, char*, char*)= dlsym(handle, "find_tnode_by_name");
    void (*sort_tree_by_phone)(Tree*)= dlsym(handle, "sort_tree_by_phone");
    TNode* (*create_tnode)(void)= dlsym(handle, "create_tnode");
    void (*insert_to_tnode)(TNode*, char*, char*, char*, char* , char*, int)= dlsym(handle, "insert_to_tnode");
    void(*print_tnode)(TNode* )= dlsym(handle, "print_tnode");
    void (*print_tree)(Tree* )= dlsym(handle, "print_tree");
    TNode* (*max_tnode)(TNode* )= dlsym(handle, "max_tnode");
    TNode* (*min_tnode)(TNode* )= dlsym(handle, "min_tnode");
    TNode* (*successor)(TNode* )= dlsym(handle, "successor");
    TNode* (*predecessor)(TNode* )= dlsym(handle, "predecessor");

//-----------------------------------------
    List* (*create_list)(void)= dlsym(handle, "create_list");
    void (*remove_list)(List*)= dlsym(handle, "remove_list");
    void (*add_to_list)(List*, LNode*)= dlsym(handle, "add_to_list");
    void (*remove_lnode)(LNode*)= dlsym(handle, "remove_lnode");
    void (*remove_from_list)(List*, LNode*)= dlsym(handle, "remove_from_list");
    void (*sort_list_by_surname)(List *)= dlsym(handle, "sort_list_by_surname");
    void (*sort_list_by_birthdate)(List *)= dlsym(handle, "sort_list_by_birthdate");
    void (*sort_list_by_phone)(List *)= dlsym(handle, "sort_list_by_phone");
    void (*swap_lnodes)(LNode *, LNode *, List *)= dlsym(handle, "swap_lnodes");
    LNode*(*find_lnode_by_name)(List *, char* , char*)= dlsym(handle, "find_lnode_by_name");
    LNode*(*create_lnode)(void) = dlsym(handle, "create_lnode");
    void (*insert_to_lnode)(LNode*, char*, char*, char*, char* , char*, int)= dlsym(handle, "insert_to_lnode");
    void (*print_lnode)(LNode*)= dlsym(handle, "print_lnode");
    void (*print_list)(List*)= dlsym(handle, "print_list");

    #endif // DLL

    struct tms *start = (struct tms*) malloc (sizeof(struct tms));
    struct tms *stop = (struct tms*) malloc (sizeof(struct tms));

	  struct timespec *start_real = (struct timespec*) malloc (sizeof(struct timespec));
	  struct timespec *stop_real = (struct timespec*) malloc (sizeof(struct timespec));


    const int N = 1000;
    srand(time(NULL));

    times(start);
    clock_gettime(CLOCK_REALTIME,start_real);
    Tree* tree = create_tree();
    times(stop);
    clock_gettime(CLOCK_REALTIME, stop_real);
    printf("\nTWORZENIE DRZEWA: \nCzas uzytkownika: %ld \n Czas systemowy: %ld \nCzas rzeczywisty: %ld s  %ld ns\n", stop->tms_utime-start->tms_utime, stop->tms_stime-start->tms_stime, (stop_real->tv_sec-start_real->tv_sec),(stop_real->tv_nsec-start_real->tv_nsec));


    times(start);
    clock_gettime(CLOCK_REALTIME,start_real);
    List* list = create_list();
    times(stop);
    clock_gettime(CLOCK_REALTIME, stop_real);
    printf("\nTWORZENIE LISTY: \nCzas uzytkownika: %ld \n Czas systemowy: %ld \nCzas rzeczywisty: %ld s  %ld ns\n", stop->tms_utime-start->tms_utime, stop->tms_stime-start->tms_stime, (stop_real->tv_sec-start_real->tv_sec),(stop_real->tv_nsec-start_real->tv_nsec));

    int i;
    for(i = 0; i < N; i++){
      times(start);
      clock_gettime(CLOCK_REALTIME,start_real);

      LNode* lnode = create_lnode();
      insert_to_lnode(lnode, firstname[rand()%93], surname1[rand()%83], birthdate1[rand()%99], email1[rand()%150], addr[rand()%99], 500000000 + (rand()%10000000));
      add_to_list(list, lnode);

      times(stop);
      clock_gettime(CLOCK_REALTIME, stop_real);
      printf("\nTWORZENIE ELEMENTU LISTY NR %d: \nCzas uzytkownika: %ld \n Czas systemowy: %ld \nCzas rzeczywisty: %ld s  %ld ns\n",
       i+1, stop->tms_utime-start->tms_utime, stop->tms_stime-start->tms_stime, (stop_real->tv_sec-start_real->tv_sec),(stop_real->tv_nsec-start_real->tv_nsec));

    }

    TNode *last;
    for (i = 0; i < N; i++){
      times(start);
      clock_gettime(CLOCK_REALTIME,start_real);

        TNode* tnode = create_tnode();
        insert_to_tnode(tnode, firstname[rand()%93], surname1[rand()%83], birthdate1[rand()%99], email1[rand()%150], addr[rand()%99], 500000000 + (rand()%10000000));
        add_to_tree(tree, tnode);
        last = tnode;
        times(stop);
        clock_gettime(CLOCK_REALTIME, stop_real);
        printf("\nTWORZENIE ELEMENTU DRZEWA NR %d: \nCzas uzytkownika: %ld \n Czas systemowy: %ld \nCzas rzeczywisty: %ld s  %ld ns\n",
        i+1, stop->tms_utime-start->tms_utime, stop->tms_stime-start->tms_stime, (stop_real->tv_sec-start_real->tv_sec),(stop_real->tv_nsec-start_real->tv_nsec));
    }

  //optymistycznie - szukany element na początku listy
	times(start);
	clock_gettime(CLOCK_REALTIME,start_real);
        print_lnode(find_lnode_by_name(list, list->first->name, list->first->surname));
	times(stop);
	clock_gettime(CLOCK_REALTIME, stop_real);
  printf("\nWYSZUKIWANIE ELEMENTU LISTY (optymistycznie) \nCzas uzytkownika: %ld \n Czas systemowy: %ld \nCzas rzeczywisty: %ld s  %ld ns\n",
  stop->tms_utime-start->tms_utime, stop->tms_stime-start->tms_stime, (stop_real->tv_sec-start_real->tv_sec),(stop_real->tv_nsec-start_real->tv_nsec));

  //pesymistycznie - szukany element na koncu listy
  times(start);
  clock_gettime(CLOCK_REALTIME,start_real);
        print_lnode(find_lnode_by_name(list, list->last->name, list->last->surname));
  times(stop);
  clock_gettime(CLOCK_REALTIME, stop_real);
  printf("\nWYSZUKIWANIE ELEMENTU LISTY (pesymistycznie) \nCzas uzytkownika: %ld \n Czas systemowy: %ld \nCzas rzeczywisty: %ld s  %ld ns\n",
  stop->tms_utime-start->tms_utime, stop->tms_stime-start->tms_stime, (stop_real->tv_sec-start_real->tv_sec),(stop_real->tv_nsec-start_real->tv_nsec));

  //optymistycznie - usuwam pierwszy element
  times(start);
  clock_gettime(CLOCK_REALTIME,start_real);
        remove_from_list(list, list->first);
  times(stop);
  clock_gettime(CLOCK_REALTIME, stop_real);
  printf("\nUSUWANIE ELEMENTU LISTY (optymistycznie) \nCzas uzytkownika: %ld \n Czas systemowy: %ld \nCzas rzeczywisty: %ld s  %ld ns\n",
   stop->tms_utime-start->tms_utime, stop->tms_stime-start->tms_stime, (stop_real->tv_sec-start_real->tv_sec),(stop_real->tv_nsec-start_real->tv_nsec));

   //pesymistycznie - ostatni element
   times(start);
   clock_gettime(CLOCK_REALTIME,start_real);
         remove_from_list(list, list->last);
   times(stop);
   clock_gettime(CLOCK_REALTIME, stop_real);
   printf("\nUSUWANIE ELEMENTU LISTY (pesymistycznie) \nCzas uzytkownika: %ld \n Czas systemowy: %ld \nCzas rzeczywisty: %ld s  %ld ns\n",
    stop->tms_utime-start->tms_utime, stop->tms_stime-start->tms_stime, (stop_real->tv_sec-start_real->tv_sec),(stop_real->tv_nsec-start_real->tv_nsec));

  //optymistycznie - root
  times(start);
  clock_gettime(CLOCK_REALTIME,start_real);

        find_tnode_by_name(tree, tree->root->name, tree->root->surname);

  times(stop);
  clock_gettime(CLOCK_REALTIME, stop_real);
  printf("\nWYSZUKIWANIE ELEMENTU DRZEWA (optymistycznie) \nCzas uzytkownika: %ld \n Czas systemowy: %ld \nCzas rzeczywisty: %ld s  %ld ns\n",
  stop->tms_utime-start->tms_utime, stop->tms_stime-start->tms_stime, (stop_real->tv_sec-start_real->tv_sec),(stop_real->tv_nsec-start_real->tv_nsec));

  //pesymistycznie - podaje imie i nazwisko ktorych nie ma w ksiazce - przeszukanie calego drzewa
  times(start);
  clock_gettime(CLOCK_REALTIME,start_real);
        find_tnode_by_name(tree, "Ktos", "Jakistam");
  times(stop);
  clock_gettime(CLOCK_REALTIME, stop_real);
  printf("\nWYSZUKIWANIE ELEMENTU DRZEWA (pesymistycznie) \nCzas uzytkownika: %ld \n Czas systemowy: %ld \nCzas rzeczywisty: %ld s  %ld ns\n",
  stop->tms_utime-start->tms_utime, stop->tms_stime-start->tms_stime, (stop_real->tv_sec-start_real->tv_sec),(stop_real->tv_nsec-start_real->tv_nsec));

  //optymistycznie - element ostatnio dodany
  times(start);
  clock_gettime(CLOCK_REALTIME,start_real);
        remove_from_tree(tree, last);
  times(stop);
  clock_gettime(CLOCK_REALTIME, stop_real);
  printf("\nUSUWANIE ELEMENTU Z DRZEWA(optymistycznie) \nCzas uzytkownika: %ld \n Czas systemowy: %ld \nCzas rzeczywisty: %ld s  %ld ns\n",
  stop->tms_utime-start->tms_utime, stop->tms_stime-start->tms_stime, (stop_real->tv_sec-start_real->tv_sec),(stop_real->tv_nsec-start_real->tv_nsec));

  //pesymistycznie - usuwanie elementu najbliżej roota
  times(start);
  clock_gettime(CLOCK_REALTIME,start_real);
        remove_from_tree(tree, tree->root->left);
  times(stop);
  clock_gettime(CLOCK_REALTIME, stop_real);
  printf("\nUSUWANIE ELEMENTU Z DRZEWA (pesymistycznie) \nCzas uzytkownika: %ld \n Czas systemowy: %ld \nCzas rzeczywisty: %ld s  %ld ns\n",
  stop->tms_utime-start->tms_utime, stop->tms_stime-start->tms_stime, (stop_real->tv_sec-start_real->tv_sec),(stop_real->tv_nsec-start_real->tv_nsec));

  //sortowanie
  times(start);
  clock_gettime(CLOCK_REALTIME,start_real);

        sort_list_by_surname(list);

  times(stop);
  clock_gettime(CLOCK_REALTIME, stop_real);
  printf("\nSORTOWANIE LISTY\nCzas uzytkownika: %ld \n Czas systemowy: %ld \nCzas rzeczywisty: %ld s  %ld ns\n",
  stop->tms_utime-start->tms_utime, stop->tms_stime-start->tms_stime, (stop_real->tv_sec-start_real->tv_sec),(stop_real->tv_nsec-start_real->tv_nsec));



    //znajdowanie
    //print_lnode(find_lnode_by_name(list, "Marek", "Kanarek"));
    //print_tnode(find_tnode_by_name(tree, "Marzena", "Nowak"));

  //  print_list(list);

    //sort_list_by_phone(list);
    //print_list(list);
    //sort_tree_by_surname(tree);
    //printf("\nWYPISANIE LISTY:\n");
    //print_list(list);
    //printf("\n\nWYPISANIE DRZEWA:\n");
    //print_tree(tree);
    //
    //printf("dalej dzialam\n");
    remove_list(list);
    remove_tree(tree);
//
    #ifdef DLL
        dlclose(handle);
    #endif
   return 0;
}
