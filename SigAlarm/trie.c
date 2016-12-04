#include <stdio.h>
#include <stdlib.h>

struct node{
    int priority;
    int rank;
    void *data;
    struct node *left;
    struct node *right;
};

struct p_node{
    struct node *first;
    struct node *second;
};

struct p_node new_p_node(struct node *fi, struct node *se){
    struct p_node tmp;
    tmp.first = fi;
    tmp.second = se;
    return tmp;
}

struct p_node split(struct node *head, int priority){
    if (head == NULL){
        return new_p_node(NULL, NULL);
    }

    if (head->priority <= priority){
        struct p_node tmp = split(head->right, priority);
        head->right = tmp.first;
        return new_p_node(head, tmp.second);
    } else {
        struct p_node tmp = split(head->left, priority);
        head->left = tmp.second;
        return new_p_node(tmp.first, head);
    }
}

struct node *merge(struct node *first, struct node *second){
    if (first == NULL){
        return second;
    }

    if (second == NULL){
        return first;
    }

    if (first->rank > second->rank){
        first->right = merge(first->right, second);
        return first;
    } else {
        second->left = merge(first, second->left);
        return second;
    }
}

struct node *new_node(int priority, void *data){
    struct node *nw = (struct node*)malloc(sizeof(struct node));
    nw->data = data;
    nw->priority = priority;
    nw->left = NULL;
    nw->right = NULL;
    nw->rank = rand();
    return nw;
}

struct node *insert(struct node *head, int priority, void *data){
    struct p_node tmp = split(head, priority);
    struct node *nw = new_node(priority, data);
    head = merge(tmp.first, nw);
    head = merge(head, tmp.second);
    return head;
}

struct node *remove_first(struct node *head){
    if (head == NULL)
        return NULL;

    if (head->left == NULL){
        return head->right;
    } else {
        head->left = remove_first(head->left);
        return head;
    }
}

void *first_elem(struct node *head){
    if (head == NULL)
        return NULL;

    if (head->left == NULL){
        return head->data;
    } else {
        return first_elem(head->left);
    }
}

