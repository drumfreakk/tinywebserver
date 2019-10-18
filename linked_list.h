#ifndef TODO_WEBAPP_LINKED_LIST_H
#define TODO_WEBAPP_LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_SIZE 20
#define VALUE_SIZE 20

typedef struct Keypair{
	char *key;
	char *value;
} data_t;

int init_data(data_t *data){
	data->key = malloc(sizeof(char) * KEY_SIZE);
	data->value = malloc(sizeof(char) * VALUE_SIZE);
	if(data->key == NULL || data->value == NULL){
		return -1;
	}
	strcpy(data->key, "");
	strcpy(data->value, "");
}

void clear_data(data_t *data){
	free(data->key);
	free(data->value);
	data->value = NULL;
	data->key = NULL;
//	strcpy(data->key, "");
//	strcpy(data->value, "");
}	//TODO safeguard these

void print_data(data_t *data){
	printf("%s\t\t-->\t%s\n", data->key, data->value);
}

void pop_data(data_t *to_clear, data_t *out){
	strcpy(out->key, to_clear->key);
	strcpy(out->value, to_clear->value);
	clear_data(to_clear);
}

typedef struct Node{
	data_t data;
	struct Node *next;
} node_t;


void print_list(node_t *head) {
	node_t * current = head;

	while (current != NULL) {
		printf("[]: ");
		print_data(&current->data);
		current = current->next;
	}
	printf("\n");
}

int init_list(node_t **head, char *key, char *value){
	node_t *new_node = NULL;
	new_node = malloc(sizeof(node_t));
	if(new_node == NULL){
		return -1;
	}
	init_data(&new_node->data);
	strcpy(new_node->data.key, key);
	strcpy(new_node->data.value, value);
	*head = new_node;
}

int add_last(node_t *head, char *key, char *value) {
	node_t * current = head;
	while (current->next != NULL) {
		current = current->next;
	}

	/* now we can add a new variable */
	current->next = malloc(sizeof(node_t));
	if(current->next == NULL){
		return -1;
	}
	init_data(&current->next->data);
	strcpy(current->next->data.key, key);
	strcpy(current->next->data.value, value);
	current->next->next = NULL;
}

int push(node_t **head, char *key, char *value) {
	node_t * new_node;
	new_node = malloc(sizeof(node_t));
	if(new_node == NULL){
		return -1;
	}
	init_data(&new_node->data);
	strcpy(new_node->data.key, key);
	strcpy(new_node->data.value, value);
	new_node->next = *head;
	*head = new_node;
}

data_t pop(node_t **head) {	//TODO memory?
	data_t retval;
	init_data(&retval);
	node_t * next_node = NULL;

	if (*head == NULL) {
		clear_data(&retval);
		return retval;
	}

	next_node = (*head)->next;
	pop_data(&(*head)->data, &retval);
	free(*head);
	*head = next_node;

	return retval;
}

data_t remove_last(node_t * head) {
	data_t retval;
	init_data(&retval);
	/* if there is only one item in the list, remove it */
	if (head->next == NULL) {
		pop_data(&head->data, &retval);
		free(head);
		return retval;
	}

	/* get to the second to last node in the list */
	node_t * current = head;
	while (current->next->next != NULL) {
		current = current->next;
	}

	/* now current points to the second to last item of the list, so let's remove current->next */
	pop_data(&current->next->data, &retval);
	free(current->next);
	current->next = NULL;
	return retval;

}

data_t remove_by_index(node_t ** head, int n) {
	int i = 0;
	data_t retval;
	init_data(&retval);
	node_t * current = *head;
	node_t * temp_node = NULL;

	if (n == 0) {
		return pop(head);
	}

	for (i = 0; i < n-1; i++) {
		if (current->next == NULL) {
			clear_data(&retval);
			return retval;
		}
		current = current->next;
	}

	temp_node = current->next;
	pop_data(&temp_node->data, &retval);

	current->next = temp_node->next;
	free(temp_node);

	return retval;

}

data_t remove_by_value(node_t ** head, char *key){
	node_t * current = *head;
	int count = 0;

	while (current != NULL) {
		if(strcmp(current->data.key, key) == 0){
			return remove_by_index(head, count);
		}
		count++;
		current = current->next;
	}
	data_t ret;
	clear_data(&ret);
	return ret;
}

void clear_list(node_t **head){
	while(pop(head).key != NULL){}
}

#endif
