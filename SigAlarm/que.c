#include <stdio.h>
#include <stdlib.h>
#include "trie.h"
struct node *head = NULL;

void push(int priority, void *data){
    head = insert(head, priority, data);
}

void pop(){
    head = remove_first(head);
}

void *first(){
    return first_elem(head);
}
