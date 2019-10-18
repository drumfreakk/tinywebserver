#ifndef TODO_WEBAPP_LINKED_LIST_H
#define TODO_WEBAPP_LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NODE

#ifdef LIST

#define KEY_SIZE 20
#define VAL_SIZE 20

struct Linked_List{		//TODO struct with key+value+pointer to next
	char **keys;
	char **values;
	int size;
};

int init_list(struct Linked_List *list, int size){
	list->size = size;
	list->keys = (char**)malloc(size * sizeof(char *));
	list->values = (char**)malloc(size * sizeof(char *));
	if(list->keys == NULL || list->values == NULL){
		return 1;
	}

	for(int i = 0; i < size; i++){
		list->keys[i] = (char*)malloc(KEY_SIZE * sizeof(char));
		list->values[i] = (char*)malloc(VAL_SIZE * sizeof(char));
		if(list->keys[i] == NULL || list->values == NULL){
			return 1;
		}
		strcpy(list->keys[i], "");
		strcpy(list->values[i], "");
	}
	return 0;
}

int resize(struct Linked_List *list, int newsize){
	if(newsize <= list->size){
		return 1;
	}
	list->keys = (char**) realloc(list->keys, newsize*(sizeof(char*)));
	list->keys = (char**) realloc(list->keys, newsize*(sizeof(char*)));

	if(list->keys == NULL || list->values == NULL){
		return 1;
	}

	for(int i = list->size; i < newsize; i++){
		list->keys[i] = (char*)malloc(KEY_SIZE * sizeof(char));
		list->values[i] = (char*)malloc(VAL_SIZE * sizeof(char));
		if(list->keys[i] == NULL || list->values == NULL){
			return 1;
		}
		strcpy(list->keys[i], "");
		strcpy(list->values[i], "");
	}

	list->size = newsize;
	return 0;
}

int getIdByKey(struct Linked_List *list, char *key){
	int len = strlen(key);
	if(len >= KEY_SIZE){
		return -1;
	}
	for(int i = 0; i < list->size; i++){
		if(strncmp(key, list->keys[i], len) == 0){
			return i;
		}
	}
}

char* getValueByKey(struct Linked_List *list, char *key){
	return list->values[getIdByKey(list, key)];
}

void end(struct Linked_List *list){
	for(int i = 0; i < list->size; i++){
		free(list->keys[i]);
		free(list->values[i]);
	}
	free(list->keys);
	free(list->values);
}

void printLinkedList(struct Linked_List *list){
	for(int i = 0; i < list->size; i++){
		if(strcmp(list->keys[i], "") == 0){
			continue;
		}
		printf("[%d]:\t%s\t\t-->\t%s\n", i, list->keys[i], list->values[i]);
	}
}

#endif
#ifdef NODE

typedef struct Node{
	int val;
	struct Node *next;
} node_t;

void print_list(node_t * head) {
	node_t * current = head;

	while (current != NULL) {
		printf("%d\n", current->val);
		current = current->next;
	}
	printf("\n");
}

int init_list(node_t ** head, int val){
	node_t *new_node = NULL;
	new_node = malloc(sizeof(node_t));
	if(new_node == NULL){
		return -1;
	}
	new_node->val = val;
	*head = new_node;
}

int add_last(node_t * head, int val) {
	node_t * current = head;
	while (current->next != NULL) {
		current = current->next;
	}

	/* now we can add a new variable */
	current->next = malloc(sizeof(node_t));
	if(current->next == NULL){
		return -1;
	}
	current->next->val = val;
	current->next->next = NULL;
}

int push(node_t ** head, int val) {
	node_t * new_node;
	new_node = malloc(sizeof(node_t));
	if(new_node == NULL){
		return -1;
	}

	new_node->val = val;
	new_node->next = *head;
	*head = new_node;
}

int pop(node_t ** head) {
	int retval = -1;
	node_t * next_node = NULL;

	if (*head == NULL) {
		return -1;
	}

	next_node = (*head)->next;
	retval = (*head)->val;
	free(*head);
	*head = next_node;

	return retval;
}

int remove_last(node_t * head) {
	int retval = 0;
	/* if there is only one item in the list, remove it */
	if (head->next == NULL) {
		retval = head->val;
		free(head);
		return retval;
	}

	/* get to the second to last node in the list */
	node_t * current = head;
	while (current->next->next != NULL) {
		current = current->next;
	}

	/* now current points to the second to last item of the list, so let's remove current->next */
	retval = current->next->val;
	free(current->next);
	current->next = NULL;
	return retval;

}

int remove_by_index(node_t ** head, int n) {
	int i = 0;
	int retval = -1;
	node_t * current = *head;
	node_t * temp_node = NULL;

	if (n == 0) {
		return pop(head);
	}

	for (i = 0; i < n-1; i++) {
		if (current->next == NULL) {
			return -1;
		}
		current = current->next;
	}

	temp_node = current->next;
	retval = temp_node->val;
	current->next = temp_node->next;
	free(temp_node);

	return retval;

}

int remove_by_value(node_t ** head, int val){
	node_t * current = *head;
	int count = 0;

	while (current != NULL) {
		if(current->val == val){
			return remove_by_index(head, count);
		}
		count++;
		current = current->next;
	}
	return -1;
}

void clear_list(node_t **head){
	while(pop(head) != -1){}
}

#endif

#endif
