#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dlfcn.h>
#include "address_book.h"

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
    void(*print_tnode)(TNode* node)= dlsym(handle, "print_tnode");
    void (*print_tree)(Tree* tree)= dlsym(handle, "print_tree");
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

    Tree* tree = create_tree();
    List* list = create_list();

    LNode* lnode1 = create_lnode();
    LNode* lnode2 = create_lnode();
    LNode* lnode3 = create_lnode();
    LNode* lnode4 = create_lnode();
    LNode* lnode5 = create_lnode();

    TNode* tnode1 = create_tnode();
    TNode* tnode2 = create_tnode();
    TNode* tnode3 = create_tnode();
    TNode* tnode4 = create_tnode();

    insert_to_tnode(tnode1,"Ania","Baranowska","1666-02-06","ania12@op.pl","Dworcowa 1/2 Krakow", 123123132);
    insert_to_tnode(tnode2,"Ktos","Co","1996-01-16","inny12@op.pl","Inna 1/12 Waarszawa", 564929384);
    insert_to_tnode(tnode3,"Marek","Kanarek","1999-12-11","konto@gmail.com","Ulicowa 33 Rzeszow", 444332212);
    insert_to_tnode(tnode4,"Marzena","Nowak","1964-11-27","marzena@gmail.com","Kolejna 55 Szczecin", 845634477);

    add_to_tree(tree, tnode1);
    add_to_tree(tree, tnode3);
    add_to_tree(tree, tnode2);
    add_to_tree(tree, tnode4);

    insert_to_lnode(lnode5,"Szczepan","Kowalski","1666-02-06","ania22@op.pl","Dworcowa 1/2 Krakow", 123444432);
    insert_to_lnode(lnode1,"Maria","Malinowska","1666-02-06","ania12@op.pl","Dworcowa 1/2 Krakow", 123123132);
    insert_to_lnode(lnode2,"Ktos","Co","1996-01-16","inny12@op.pl","Inna 1/12 Waarszawa", 564929384);
    insert_to_lnode(lnode3,"Marek","Kanarek","1999-12-11","konto@gmail.com","Ulicowa 33 Rzeszow", 444332212);
    insert_to_lnode(lnode4,"Marzena","Nowak","1964-11-27","marzena@gmail.com","Kolejna 55 Szczecin", 845634477);

    add_to_list(list, lnode2);
    add_to_list(list, lnode3);
    add_to_list(list, lnode1);
    add_to_list(list, lnode4);
    add_to_list(list, lnode5);

  //  print_list(list);
  //  print_tree(tree);
    //znajdowanie
    print_lnode(find_lnode_by_name(list, "Marek", "Kanarek"));
    print_tnode(find_tnode_by_name(tree, "Marzena", "Nowak"));

    remove_from_list(list, lnode4);
    remove_from_tree(tree, tnode3);

  //  print_list(list);

    sort_list_by_phone(list);
    //print_list(list);
    //sort_tree_by_surname(tree);
    printf("WYPISANIE LISTY:\n");
    print_list(list);
    printf("\nWYPISANIE DRZEWA:\n");
    print_tree(tree);
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
