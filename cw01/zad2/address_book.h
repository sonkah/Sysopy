#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef ADDRESS_BOOK
#define ADDRESS_BOOK

typedef struct LNode{

    int phone;
    char* name;
    char* surname;
    char* birth_date;
    char* email;
    char* address;

    struct LNode* next;
    struct LNode* prev;

}LNode;

typedef struct TNode{
    char* name;
    char* surname;
    char* birth_date;
    char* email;
    int phone;
    char* address;

    struct TNode* left;
    struct TNode* right;
    struct TNode* parent;
}TNode;


typedef struct Tree{
    TNode* root;
}Tree;

typedef struct List{
    LNode* first;
    LNode* last;
}List;

//-----------------------------------------

Tree* create_tree(void);

void remove_tree(Tree* tree);
void t_removal(TNode* node);

void remove_leaf(TNode* tnode);

void add_to_tree(Tree* tree, TNode* node);
void add_to_tree_rec(TNode*current, TNode*node);

TNode* remove_from_tree(Tree* tree, TNode* node);
TNode * t_remove(TNode * root, TNode * node);
TNode* max_tnode(TNode* node);
TNode* min_tnode(TNode* node);
TNode* successor(TNode* node);
TNode* predecessor(TNode* node);

TNode* find_tnode_by_name(Tree* tree, char* name, char* surname);
TNode* ftbn(char* name, char* surname, TNode* node);

void sort_tree_by_phone(Tree* tree);

TNode* create_tnode(void);



void insert_to_tnode(TNode*node, char*name, char*surname, char*birth_date, char* email, char*address, int phone);

void print_tnode(TNode* node);
void print_tree(Tree* tree);
void print_tree_rec(TNode* node);

//-----------------------------------------

List* create_list(void);
void remove_list(List* l);


void add_to_list(List* list, LNode* node);

void remove_lnode(LNode* node);

void remove_from_list(List*list, LNode* node);

void sort_list_by_surname(List *list);
void sort_list_by_birthdate(List *list);
void sort_list_by_phone(List *list);
void swap_lnodes(LNode *node1, LNode *node2, List *list);

LNode* find_lnode_by_name(List * list, char* name, char*surname);

LNode* create_lnode(void);

void insert_to_lnode(LNode*node, char*name, char*surname, char*birth_date, char* email, char*address, int phone);

void print_lnode(LNode* node);
//-----------------------------------------

void print_list(List*list);
#endif
