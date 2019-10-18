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

struct Node{
	char *key;
	char *data;
	struct Node *next;
};



void insertNew(struct Node *head, char *key, char *data){
	struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
//	newNode->key = malloc(sizeof(char) * 20);
//	newNode->data = malloc(sizeof(char) * 20);
//	strcpy(newNode->key, key);				//TODO Fix the sigabrt
//	strcpy(newNode->data, data);

	newNode->next = head;
	head = newNode;
//	printf("New: %x: %s\t-->\t%s\t@\t%x\n", head->next, head->key, head->data, head);
}

void removeHead(struct Node *head){
	struct Node *newHead = head->next;
//	free(head->key);
//	free(head->data);
	free(head);
	head = newHead;
}

void removeList(struct Node *head){
	while(head->next != NULL){
		removeHead(head);
	}
}

void printList(struct Node *head){
	struct Node *current = head;
	while(current != NULL){
//		printf("%x: %s\t-->\t%s\t\t%x\n", current, current->key, current->data, current->next);
		printf("%x:%x\n", current, current->next);
//		printf("%s\t-->\t%s\n", current->key, current->data);
		current = current->next;	//TODO fix double last
	}
}

#endif

#endif
