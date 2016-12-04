#include <stdio.h>
#include <stdlib.h>
#include "trie.c"

struct node;
struct p_node;
struct p_node new_p_node(struct node *fi, struct node *se);
struct p_node split(struct node *head, int priority);
struct node *merge(struct node *first, struct node *second);
struct node *new_node(int priority, void *data);
struct node *insert(struct node *head, int priority, void *data);
struct node *remove_first(struct node *head);
void *first_elem(struct node *head);
void print(struct node *head, int k);
int size(struct node* head);
