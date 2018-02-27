#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "address_book.h"


//-------------------------------------tree

Tree* create_tree(void){
    Tree* tree = (Tree*)malloc(sizeof(Tree));
    tree -> root = NULL;
    return tree;
}
//-----------------------------------------

void remove_tree(Tree* tree){
    if(tree == NULL){
        return;
    }
    t_removal(tree->root);
}

void t_removal(TNode* node){
    if(node != NULL){
        t_removal(node->left);
        t_removal(node->right);
        remove_leaf(node);
    }
}

void remove_leaf(TNode* tnode){
    if(tnode->parent!=NULL && tnode->parent->left == tnode){
        tnode->parent->left = NULL;
    }
    else if(tnode->parent!=NULL && tnode->parent->right == tnode){
        tnode->parent->right = NULL;
    }
    free(tnode);
    return;
}
//-----------------------------------------

void add_to_tree(Tree* tree, TNode* node){
    if(node == NULL|| tree==NULL)
        return;
    if(tree -> root == NULL){
        tree->root = node;
        node -> left = NULL;
        node -> right = NULL;
        node -> parent = NULL;
    }
    else{
        add_to_tree_rec(tree->root, node); //dodaje rekurencyjnie w porz¹dku alfabetycznym po nazwisku
    }
}

void add_to_tree_rec(TNode*current, TNode*node){

    if(strcmp(current->surname, node->surname)>=0){
        if(current -> left == NULL){
            current->left = node;
            node->parent=current;
            node ->left = NULL;
            node->right = NULL;
            return;
        }
        else{
            add_to_tree_rec(current->left, node);
        }
    }
    else if(strcmp(current->surname, node->surname)<0){
        if(current->right == NULL){
            current->right = node;
            node->parent = current;
            node->left=NULL;
            node->right = NULL;
            return;
        }
        else{
            add_to_tree_rec(current->right, node);
        }
    }
}

//-----------------------------------------

TNode* max_tnode(TNode* node){
    while(node->right != NULL)
        node = node->right;
    return node;
}

TNode* min_tnode(TNode* node){
    while(node->left != NULL)
        node = node->left;
    return node;
}

TNode* successor(TNode* node){
    if(node->right != NULL)
        return min_tnode(node->right);
    else{
        TNode* tmp = node->parent;
        while(tmp!=NULL && node==tmp->right){
            node = tmp;
            tmp = tmp->parent;
        }
        return tmp;
    }
}

TNode* predecessor(TNode* node){
    if(node->left != NULL)
        return max_tnode(node->left);
    else{
        TNode* tmp = node->parent;
        while (tmp != NULL && node == tmp->left){
            node = tmp;
            tmp = tmp->parent;
        }
        return tmp;
    }
}
//-----------------------------------------

TNode* ftbn(char* name, char* surname, TNode* node){
    if( strcmp(node->name,name) == 0 && strcmp(node->surname,surname) == 0){
            return node;
    }
    TNode* left_res = NULL;
    TNode* right_res = NULL;
    if(node->left!=NULL){
        left_res = ftbn(name, surname, node->left);
    }
    if(node->right!=NULL){
        right_res = ftbn(name, surname, node->right);
    }
    if(left_res !=NULL) return left_res;
    else return right_res;
}

TNode* find_tnode_by_name(Tree* tree, char* name, char* surname){
    TNode* node = tree->root;
    TNode* x = ftbn(name, surname, node);
    if (x == NULL)
        printf("Nie kontaktu o takim nazwisku.");
    return x;
}
//-----------------------------------------

TNode * remove_from_tree(Tree * tree, TNode * node){
    return t_remove(tree->root, node);
}


TNode * t_remove(TNode * root, TNode * node){
    TNode * y = node->parent, * z;

    if(node->left!=NULL && node->right!=NULL){
        z = t_remove(root, predecessor(node));
        z->left = node->left;
        if(z->left!=NULL)
            z->left->parent = z;
        z->right = node->right;
        if(z->right!=NULL)
            z->right->parent = z;
    }
    else if(node->left!=NULL){
        z = node->left;
    }
    else z = node->right;

    if(z!=NULL) z->parent = y;

    if(y==NULL) root = z;
    else if(y->left == node) y->left = z;
    else y->right = z;

    return node;
}
//-----------------------------------------
void sort_tree_by_surname(Tree* tree){
    Tree * tmp = create_tree();

}
//-----------------------------------------

TNode* create_tnode(){
    TNode* node = (TNode*)malloc(sizeof(TNode));
    node -> name = NULL;
    node -> surname = NULL;
    node -> birth_date = NULL;
    node -> email = NULL;
    node -> phone = 0;
    node -> address = NULL;

    node -> left = NULL;
    node -> right = NULL;
    node -> parent = NULL;

    return node;
}
//-----------------------------------------

void insert_to_tnode(TNode*node, char*name, char*surname, char*birth_date, char* email, char*address, int phone){
    node -> name = name;
    node -> surname = surname;
    node -> birth_date = birth_date;
    node -> email = email;
    node -> phone = phone;
    node -> address = address;
}
//-----------------------------------------

void print_tnode(TNode* node){
    printf("IMIE: %s\n", node->name);
    printf("NAZWISKO: %s\n", node->surname);
    printf("DATA URODZENIA: %s\n", node->birth_date);
    printf("EMAIL: %s\n", node->email);
    printf("TELEFON: %d\n", node->phone);
    printf("ADRES: %s\n", node->address);
    printf("--------------------------------\n\n");
}
//-----------------------------------------

void print_tree(Tree* tree){
    if(tree==NULL){
        printf("nie ma drzewa");
        return;
    }
    print_tree_rec(tree->root);
}

void print_tree_rec(TNode* node){
    if(node!=NULL){
        print_tree_rec(node->left);
        print_tnode(node);
        print_tree_rec(node->right);
    }
}

/*---------------------------------------------------------
***********************************************************
---------------------------------------------------------*/

List* create_list(){
    List* list = (List* )malloc(sizeof(List));
    list->first = NULL;
    list->last = NULL;
    return list;
}
//-----------------------------------------

void remove_list(List* l){
    if(l == NULL)
        return;
    else{
        LNode* p = l->first;
        LNode* q;
        while(1){
            q = p;
            p = p->next;
            remove_lnode(q);
        }
        free(l);
    }
}
//-----------------------------------------

void add_to_list(List* list, LNode* node){
    if(list == NULL || node == NULL){
        return;
    }
    if(list->first==NULL){
        list -> first = node;
        list -> last = node;
        node -> next = NULL;
        node -> prev = NULL;
    }
    else{
        LNode* tmp = list->first;
        node->next = tmp;
        tmp->prev = node;
        node->prev = NULL;
        list->first = node;
    }
}
//-----------------------------------------

void remove_lnode(LNode* node){

    LNode* tmp;
    if(node -> prev!=NULL){
        tmp = node -> prev;
        tmp -> next = node -> next;
    }
    if(node -> next!= NULL){
        tmp = node-> next;
        tmp -> prev = node -> prev;

    }
    free(node);
}
//-----------------------------------------

void remove_from_list(List*list, LNode* node){
    if(node == NULL || list == NULL){
        return;
    }
    LNode* ptr = list -> first;

    while(ptr!=NULL && ptr != node){
        ptr = ptr -> next;
    }
    if(ptr != NULL){
        remove_lnode(ptr);
    }
}
//-----------------------------------------

LNode* find_lnode_by_name(List * list, char* name, char*surname){
    LNode* tmp = list->first;

    while(tmp!=NULL && (strcmp(tmp->name, name)!=0 && strcmp(tmp->surname, surname)!=0)){
        tmp=tmp->next;
    }
    return tmp;
}
//-----------------------------------------
//nie dziala
/*
void sort_list_by_name(List* list){

    LNode* pivot = list->last;
    LNode* left = list->first;
    LNode* right = list->last->next;

    while(left != right){
        if(strcmp(pivot->surname, left->surname)<0){
            left=left->next;
        }
        if(strcmp(pivot->surname, right->surname)>0){
            right = right->prev;
        }
        if(strcmp(pivot->surname, left->surname)>0
                && strcmp(pivot->surname, right->surname)<0){

            LNode* tmp1 = left;
            LNode* tmp2 = right;

            left = left->next;
            right = right->prev;

            tmp1->next = right->next;
            tmp2->prev = left->prev;

            tmp1->prev = right;
            tmp2->next = left;
        }
    }

}

void sort_list_by_birthdate(){

}

void sort_list_by_email(){

}

void sort_list_by_phone(){

}
*/
//-----------------------------------------

LNode* create_lnode(){
    LNode* node = (LNode*)malloc(sizeof(LNode));

    node -> name = NULL;
    node -> surname = NULL;
    node -> birth_date = NULL;
    node -> email = NULL;
    node -> phone = 0;
    node -> address = NULL;

    node -> next = NULL;
    node -> prev = NULL;

    return node;
}
//-----------------------------------------

void insert_to_lnode(LNode*node, char*name, char*surname, char*birth_date, char* email, char*address, int phone){
    node -> name = name;
    node -> surname = surname;
    node -> birth_date = birth_date;
    node -> email = email;
    node -> phone = phone;
    node -> address = address;
}
//-----------------------------------------

void print_lnode(LNode* node){
    printf("IMIE: %s\n", node->name);
    printf("NAZWISKO: %s\n", node->surname);
    printf("DATA URODZENIA: %s\n", node->birth_date);
    printf("EMAIL: %s\n", node->email);
    printf("TELEFON: %d\n", node->phone);
    printf("ADRES: %s\n", node->address);
    printf("--------------------------------\n\n");
}
//-----------------------------------------

void print_list(List*list){
    if(list==NULL)
        printf("Lista jest NULLem.");
    else{
        LNode* ptr = list->first;
        while(ptr!=NULL){
            print_lnode(ptr);
            ptr=ptr->next;
        }
    }
}
//-----------------------------------------

void sort_list_by_surname(List *list) {
    if(list != NULL) {
        LNode *head = list->first;
        LNode *current, *tmp;

        while(head != NULL) {
            current = head->next;
            while(current != NULL && strcmp(head->surname, current->surname) <= 0) {
                current = current->next;
            }
            if(current != NULL) {
                tmp = head->next;
                swap_lnodes(head, current, list);
                head = tmp;
            } else {
                head = head->next;
            }
        }
    }
}

void sort_list_by_birthdate(List *list) {
    if(list != NULL) {
        LNode *head = list->first;
        LNode *current, *tmp;

        while(head != NULL) {
            current = head->next;
            while(current != NULL && strcmp(head->birth_date, current->birth_date) <= 0) {
                current = current->next;
            }
            if(current != NULL) {
                tmp = head->next;
                swap_lnodes(head, current, list);
                head = tmp;
            } else {
                head = head->next;
            }
        }
    }
}

void sort_list_by_phone(List *list) {
    if(list != NULL) {
        LNode *head = list->first;
        LNode *current, *tmp;

        while(head != NULL) {
            current = head->next;
            while(current != NULL && head->phone - current->phone <= 0) {
                current = current->next;
            }
            if(current != NULL) {
                tmp = head->next;
                swap_lnodes(head, current, list);
                head = tmp;
            } else {
                head = head->next;
            }
        }
    }
}


void swap_lnodes(LNode *node1, LNode *node2, List *list) {
    if((node1 != NULL && node2 != NULL) && (node1 != node2)) {
        LNode *tmp1, *tmp2;

        if(list->first == node1) {
            list->first = node2;

        } else if(list->first == node2) {
            list->first = node1;
        }

        if(list->last == node1) {
            list->last = node2;

        } else if(list->last == node2) {
            list->last = node1;
        }
        tmp1 = node1->next;
        tmp2 = node2->next;
        node1->next = tmp2;
        node2->next = tmp1;
        if(tmp1 != NULL) tmp1->prev = node2;
        if(tmp2 != NULL) tmp2->prev = node1;
        tmp1 = node1->prev;
        tmp2 = node2->prev;
        node1->prev = tmp2;
        node2->prev = tmp1;
        if(tmp1 != NULL) tmp1->next = node2;
        if(tmp2 != NULL) tmp2->next = node1;
    }
}
