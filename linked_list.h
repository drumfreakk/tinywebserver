#ifndef TODO_WEBAPP_LINKED_LIST_H
#define TODO_WEBAPP_LINKED_LIST_H

#define KEY_SIZE 20
#define VALUE_SIZE 20

typedef struct Keypair{
	char *key;
	char *value;
} data_t;

int checkDataValidity(data_t *data);

void clear_data(data_t *data);

//TODO: check is NULL?
int init_data(data_t *data, char *key, char *value);

void print_data(data_t *data);

void pop_data(data_t *to_clear, data_t *out);

typedef struct Node{
	data_t data;
	struct Node *next;
} node_t;


void print_list(node_t *head);

int init_list(node_t **head, char *key, char *value);

int add_last(node_t *head, char *key, char *value);

int push(node_t **head, char *key, char *value);

//TODO memory?
data_t pop(node_t **head);

data_t remove_last(node_t * head);

data_t remove_by_index(node_t ** head, int n);

data_t remove_by_value(node_t ** head, char *key);

void clear_list(node_t **head);

#endif
