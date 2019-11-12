#ifndef TODO_WEBAPP_LINKED_LIST_H
#define TODO_WEBAPP_LINKED_LIST_H

#define KEY_SIZE 200
#define VALUE_SIZE 200

typedef struct Keypair{
	char *key;
	char *value;
} data_t;

int checkDataValidity(data_t *data);

void clear_data(data_t *data);

int init_data(data_t *data, char *key, char *value);

void print_data(data_t *data);

void pop_data(data_t *to_clear, data_t *out);

int stringToData(data_t *data, char *string, char *splitData);

typedef struct Node{
	data_t data;
	struct Node *next;
} node_t;


void print_list(node_t *head);

int init_list(node_t **head);

int add_last(node_t *head, char *key, char *value);

int push(node_t **head, char *key, char *value);

int pop(node_t **head, data_t *ret);

int remove_last(node_t * head, data_t *ret);

int remove_by_index(node_t ** head, data_t *ret, int n);

int remove_by_key(node_t ** head, data_t *ret, char *key);

int get_data_by_key(node_t *head, data_t **ret, char *key);

int get_data_by_index(node_t *head, data_t **ret, int n);

int get_index_by_key(node_t *head, char *key);

void clear_list(node_t **head);


#endif
