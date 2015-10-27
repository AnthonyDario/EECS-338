#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

struct node {
	int value;
	struct node * next;
};

struct node* insert(int value, struct node * root);

struct node* remove_front(struct node * root);

void destroy_list(struct node * root);

#endif
