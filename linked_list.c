#include "linked_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkDataValidity(data_t *data){
	return (data->key != NULL && data->value != NULL);
}

void clear_data(data_t *data){
	if(data->key != NULL){
		free(data->key);
	}
	if(data->value != NULL){
		free(data->value);
	}
	data->value = NULL;
	data->key = NULL;
}

int init_data(data_t *data, char *key, char *value){
	data->key = malloc(sizeof(char) * KEY_SIZE);
	data->value = malloc(sizeof(char) * VALUE_SIZE);
	if(data->key == NULL || data->value == NULL){
		return -1;
	}
	strcpy(data->key, key);
	strcpy(data->value, value);
}

void print_data(data_t *data){
	if(checkDataValidity(data)){
		printf("%s\t\t-->\t%s\n", data->key, data->value);
	}
}

void pop_data(data_t *to_clear, data_t *out){
	if(checkDataValidity(to_clear)){
		if(checkDataValidity(out)){
			strcpy(out->key, to_clear->key);
			strcpy(out->value, to_clear->value);
		}else{
			init_data(out, to_clear->key, to_clear->value);
		}
		clear_data(to_clear);
	} else{
		clear_data(out);
	}
}

void print_list(node_t *head){
	node_t * current = head;

	while (current != NULL) {
		printf("[]: ");
		print_data(&current->data);
		current = current->next;
	}
	printf("\n");
}

int init_list(node_t **head){
	node_t *new_node = NULL;
	new_node = malloc(sizeof(node_t));
	if(new_node == NULL){
		return -1;
	}
	init_data(&new_node->data, "", "");
//	strcpy(new_node->data.key, key);
//	strcpy(new_node->data.value, value);
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
	init_data(&current->next->data, key, value);
//	strcpy(current->next->data.key, key);
//	strcpy(current->next->data.value, value);
	current->next->next = NULL;
}

int push(node_t **head, char *key, char *value){
	node_t * new_node;
	new_node = malloc(sizeof(node_t));
	if(new_node == NULL){
		return -1;
	}
	init_data(&new_node->data, key, value);
//	strcpy(new_node->data.key, key);
//	strcpy(new_node->data.value, value);
	new_node->next = *head;
	*head = new_node;
}

//TODO memory?
data_t pop(node_t **head){
	data_t retval;
	init_data(&retval, "", "");
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

data_t remove_last(node_t * head){
	data_t retval;
	init_data(&retval, "", "");
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

data_t remove_by_index(node_t ** head, int n){
	int i = 0;
	data_t retval;
	init_data(&retval, "", "");
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

data_t get_by_value(node_t *head, char*key){
	node_t *current = head;
	while(current != NULL){
		if(strcmp(current->data.key, key) == 0){
			return current->data;
		}
		current = current->next;
	}
	data_t ret;
	printf("hi");
	clear_data(&ret);
	return ret;
}

void clear_list(node_t **head){
	while(pop(head).key != NULL){}
}

//TODO limits
int stringToData(data_t *data, char *string, char *splitData){
	clear_data(data);
	init_data(data, "", "");

	int matched = 0, splitLen = strlen(splitData);

	for(int i = 0; i < strlen(string); i++){
		if(string[i] == splitData[matched]){
			matched++;
			if(matched == splitLen){
				data->key[i+1-splitLen] = '\0';
				strcpy(data->value, &string[i+1]);
				break;
			}
		}else{
			matched = 0;
		}
		data->key[i] = string[i];
	}
	return 0;
}

