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

#define KEY_SIZE 20
#define VALUE_SIZE 20

typedef struct Keypair{		//TODO Remove keypair
	char key[KEY_SIZE];
	char value[VALUE_SIZE];
} data_t;

int init_data(data_t *data){
//	data->key = malloc(sizeof(char) * KEY_SIZE);
//	data->value = malloc(sizeof(char) * VALUE_SIZE);
//	if(data->key == NULL || data->value == NULL){
//		return -1;
//	}
	strcpy(data->key, "");
	strcpy(data->value, "");
}

void clear_data(data_t *data){
//	free(data->key);
//	free(data->value);
//	data->value = NULL;
//	data->key = NULL;
	strcpy(data->key, "");
	strcpy(data->value, "");
}	//TODO safeguard these

void print_data(data_t *data){
	printf("%s\t\t-->\t%s\n", data->key, data->value);
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
//	init_data(new_node->data);
//	new_node->data->key = malloc(sizeof(char) * KEY_SIZE);		//TODO function
//	new_node->data->value = malloc(sizeof(char) * VALUE_SIZE);
//	if(new_node->data->key == NULL || new_node->data->value == NULL){
//		return -1;
//	}
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
//	current->next->data->key = malloc(sizeof(char) * KEY_SIZE);
//	current->next->data->value = malloc(sizeof(char) * VALUE_SIZE);
//	if(current->next->data->key == NULL || current->next->data->value == NULL){
//		return -1;
//	}
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

//	new_node->data->key = malloc(sizeof(char) * KEY_SIZE);
//	new_node->data->value = malloc(sizeof(char) * VALUE_SIZE);
//	if(new_node->data->key == NULL || new_node->data->value == NULL){
//		return -1;
//	}
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
	strcpy(retval.key, (*head)->data.key);			//TODO functionize
	strcpy(retval.value, (*head)->data.value);
	clear_data(&(*head)->data);
	free(*head);
	*head = next_node;

	return retval;
}

data_t remove_last(node_t * head) {
	data_t retval;
	init_data(&retval);
	/* if there is only one item in the list, remove it */
	if (head->next == NULL) {
		strcpy(retval.key, head->data.key);			//TODO functionize
		strcpy(retval.value, head->data.value);
		clear_data(&head->data);
		free(head);
		return retval;
	}

	/* get to the second to last node in the list */
	node_t * current = head;
	while (current->next->next != NULL) {
		current = current->next;
	}

	/* now current points to the second to last item of the list, so let's remove current->next */
	strcpy(retval.key, current->next->data.key);			//TODO functionize
	strcpy(retval.value, current->next->data.value);
	clear_data(&current->next->data);
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
	strcpy(retval.key, temp_node->data.key);			//TODO functionize
	strcpy(retval.value, temp_node->data.value);
	clear_data(&temp_node->data);

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
	while(strcmp(pop(head).key, "") != 0){}
}

#endif

#endif
