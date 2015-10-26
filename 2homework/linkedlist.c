#include <stdio.h>
#include <stdlib.h>

struct node {
	int value;
	struct node * next;
};

// inserts a value at the end of the list
struct node* insert(int value, struct node * root) {

	// make the new node
	struct node * new_node;
	new_node = (struct node *) malloc(sizeof(struct node));
	new_node->value = value;
	new_node->next = 0;

	struct node * current_node = root;

	// iterate to the last node
	while(current_node->next != 0) {
		current_node = current_node->next;
	}

	// insert the node and return it
	current_node->next = new_node;
	return new_node;
}

// removes the first node in a linked list
struct node* remove_front(struct node * root) {

	struct node * new_root = root->next;
	free(root);
	return new_root;
}

// frees all the memory in the list
void destroy_list(struct node * root) {

	struct node * current_node = root;
	struct node * next_node = root->next;

	while (next_node != 0) { 
		free(current_node);
		current_node = next_node;
		next_node = current_node->next;
	}

	free(current_node);

}

int main(int argc, char *argv[]) {

	/*
	struct node * root;

	root = (struct node *) malloc(sizeof(struct node));

	root->value = 1;
	root->next = 0;

	int i;
	for (i = 2; i < 10; i++) {
		printf("inserting node %d\n", i);
		insert(i, root);
	}

	struct node * current_node = root;
	printf("%3d\n", current_node->value);

	while (current_node->next != 0) {
		current_node = current_node->next;
		printf("%3d\n", current_node->value);
	}

	root = remove_front(root);
	root = remove_front(root);


	current_node = root;

	while (current_node->next != 0) {
		printf("%3d\n", current_node->value);
		current_node = current_node->next;
	}

	destroy_list(root);
	*/

}
