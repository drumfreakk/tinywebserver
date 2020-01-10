#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "hacking.h"
#include "hacking-network.h"
#include "linked_list.h"

//#define TEST_LIST

#ifdef TEST_LIST
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
#endif

#ifndef TEST_LIST

#define PORT 8080   // the port users will be connecting to
#define WEBROOT "/home/kip/CLionProjects/tinywebserver/webroot" // the web server's root directory

void handle_connection(int, struct sockaddr_in *); // handle web requests
int get_file_size(int); // returns the filesize of open file descriptor

int main(void) {

//	struct Linked_List list;
//
//	init_list(&list, 5);
//	strcpy(list.keys[0], "hello");
//	strcpy(list.keys[1], "qwertyuiopasdfghjklzxcv");
//	printf("keyA: %s\tkeyB: %s\tkeyC: %s\n", list.keys[0], list.keys[1], list.keys[2]);
//
//
//	end(&list);
//	return 0;

	int sockfd, new_sockfd, yes=1;
	struct sockaddr_in host_addr, client_addr;   // my address information
	socklen_t sin_size;

	printf("Accepting web requests on port %d\n", PORT);

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		fatal("in socket");

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		fatal("setting socket option SO_REUSEADDR");

	host_addr.sin_family = AF_INET;      // host byte order
	host_addr.sin_port = htons(PORT);    // short, network byte order
	host_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
	memset(&(host_addr.sin_zero), '\0', 8); // zero the rest of the struct

	if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
		fatal("binding to socket");

	if (listen(sockfd, 20) == -1)
		fatal("listening on socket");

	while(1) {   // Accept loop
		sin_size = sizeof(struct sockaddr_in);
		new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
		if(new_sockfd == -1)
			fatal("accepting connection");

		handle_connection(new_sockfd, &client_addr);
	}
	return 0;
}

/* This function handles the connection on the passed socket from the
 * passed client address.  The connection is processed as a web request
 * and this function replies over the connected socket.  Finally, the
 * passed socket is closed at the end of the function.
 */
void handle_connection(int sockfd, struct sockaddr_in *client_addr_ptr) {
	unsigned char *ptr, request[500], resource[500], header[500], content[500];
	int fd, length, contentLength;
	enum HTTPType type;
	node_t *headers = NULL;

	length = recv_line(sockfd, request);
	//dump(request, length);
	printf("Got request from %s:%d \"%s\"\n", inet_ntoa(client_addr_ptr->sin_addr), ntohs(client_addr_ptr->sin_port), request);

	ptr = strstr(request, " HTTP/"); // search for valid looking request
	if(ptr == NULL) { // then this isn't valid HTTP
		printf(" NOT HTTP!\n");
	} else {
		*ptr = 0; // terminate the buffer at the end of the URL
		ptr = NULL; // set ptr to NULL (used to flag for an invalid request)
		if(strncmp(request, "GET ", 4) == 0){  // get request
			ptr = request + 4; // ptr is the URL
			type = GET;
		}
		if(strncmp(request, "HEAD ", 5) == 0){ // head request
			ptr = request + 5; // ptr is the URL
			type = HEAD;
		}
		if(strncmp(request, "POST ", 5) == 0){
			ptr = request + 5;
			type = POST;

			char value[100];
			int i, matched = 0, count = 0;
			init_list(&headers);
			while(recv_line(sockfd, header)){       //TODO: seems to infinitely loop forever receiving the headers
//				push(&headers, "", "");
//				stringToData(&headers->data, header, ": ");

				for(i = 0; i < strlen(header); i++){
//					printf("Header[%d]: %c\n", i, header[i]);
					if(header[i] == ": "[matched]){
						matched++;
//						printf("I:%d\n", i);
						if(matched == 2){
//							printf("%d\n", i);
							value[i-1] = '\0';
							push(&headers, value, &header[i+1]);
//							insertNew(value, &header[i + 1]);

							break;
						}
					}else{
						matched = 0;
					}
					value[i] = header[i];
				}
			}
			print_list(headers);

			data_t *data = NULL;
			get_data_by_key(headers, &data, "Content-Length");

			contentLength = atoi(data->value);
			char *ptr = content;
//			recv(sockfd, content, contentLength, 0);
			for(i = 0; i < contentLength; i++){
				recv(sockfd, ptr, 1, 0);
				ptr++;
			}
			*ptr = '\0';
			dump(content, contentLength);
			printf("Content-Length: %d\nContent: %s\n", contentLength, content);
		}
		if(ptr == NULL) { // then this is not a recognized request
			printf("\tUNKNOWN REQUEST!\n");
		} else { // valid request, with ptr pointing to the resource name
			if (ptr[strlen(ptr) - 1] == '/')  // for resources ending with '/'
				strcat(ptr, "index.html");     // add 'index.html' to the end
			strcpy(resource, WEBROOT);     // begin resource with web root path
			strcat(resource, ptr);         //  and join it with resource path
			fd = open(resource, O_RDONLY, 0); // try to open the file
			printf("\tOpening \'%s\'\t", resource);
			if(fd == -1) { // if file is not found
				printf(" 404 Not Found\n");
				send_string(sockfd, "HTTP/1.0 404 NOT FOUND\r\n");
				send_string(sockfd, "Server: Tiny webserver\r\n\r\n");
				send_string(sockfd, "<html><head><title>404 Not Found</title></head>");
				send_string(sockfd, "<body><h1>URL not found</h1></body></html>\r\n");
			} else {      // otherwise, serve up the file
				printf(" 200 OK\n");
				send_string(sockfd, "HTTP/1.0 200 OK\r\n");
				send_string(sockfd, "Server: Tiny webserver\r\n\r\n");
				if(type == GET || type == POST) { // then this is a GET request
					if( (length = get_file_size(fd)) == -1)
						fatal("getting resource file size");
					if( (ptr = (unsigned char *) malloc(length)) == NULL)
						fatal("allocating memory for reading resource");
					read(fd, ptr, length); // read the file into memory	//TODO:
					send(sockfd, ptr, length, 0);  // send it to socket
					free(ptr); // free file memory
				}
				close(fd); // close the file		//TODO:
			} // end if block for file found/not found
		} // end if block for valid request
	} // end if block for valid HTTP
	shutdown(sockfd, SHUT_RDWR); // close the socket gracefully
	clear_list(&headers);

}

/* This function accepts an open file descriptor and returns
 * the size of the associated file.  Returns -1 on failure.
 */
int get_file_size(int fd) {
	struct stat stat_struct;

	if(fstat(fd, &stat_struct) == -1)
		return -1;
	return (int) stat_struct.st_size;
}

#endif
