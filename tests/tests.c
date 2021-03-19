#include <stdlib.h>
#include "linked_list.h"

int main(void){
	data_t data;
	init_data(&data, "", "");
	node_t * head = NULL;

	init_list(&head);
	print_list(head);
	strcpy(head->data.key, "1");
	strcpy(head->data.value, "a");
	print_list(head);

	push(&head, "0", "b");
	print_list(head);

	add_last(head, "2", "c");
	add_last(head, "3", "d");
	add_last(head, "4", "e");
	add_last(head, "5", "f");
	add_last(head, "6", "g");
	add_last(head, "7", "h");
	print_list(head);

	printf("Pop:");
	pop(&head, &data);
	print_data(&data);
	clear_data(&data);
	print_list(head);

	printf("Last:");
	remove_last(head, &data);
	print_data(&data);
	clear_data(&data);
	print_list(head);

	printf("Index(pop):");
	remove_by_index(&head, &data, 2);
	print_data(&data);
	clear_data(&data);
	print_list(head);

	data_t *data_pointer = NULL;

	printf("Index(keep):");
	get_data_by_index(head, &data_pointer, 3);
	print_data(data_pointer);
	print_list(head);

	printf("Value(pop):");
	remove_by_key(&head, &data, "5");
	print_data(&data);
	clear_data(&data);
	print_list(head);

	printf("Value(keep):");
	get_data_by_key(head, &data_pointer, "4");
	print_data(data_pointer);
	print_list(head);

	printf("Index:%d\n", get_index_by_key(head, "2"));
	print_list(head);

	stringToData(&head->data, "hello::s:friend:::hi", ":::");
	print_list(head);

	clear_list(&head);

	return 0;
}

