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

//struct Test{
//	int s;
////	int *f;
//};
//
//void test(struct Test *a){
//	printf("%d\n", a->s);
////	printf("%d\n", (int)a->f);
//	a->s = 7;
//}
//
//void testN(struct Test *a){
////	a = malloc(sizeof(struct Test));
//	a->s = 8;
//}
//
//int main(void){
////	struct Node *head = NULL;
////	insertNew(head, "Key", "Data");
//////	insertNew(head, "Key2", "Data2");
////	removeHead(head);
////	insertNew(head, "Key3", "Data3");
////	printList(head);
////	removeList(head);
//
//int z = 2;
////	struct Test g;
////	g.s = 4;
////	g.f = z;
//	struct Test d;
////	d = &g;
//
//
////	test(d);
////	printf("%d\n", d->s);
//	testN(&d);
//z = 6;
////	d->f = z;
//	test(&d);
//	d.s = 9;
//	test(&d);
////	free(&d);
//
//	return 0;
//}



#define PORT 8080   // the port users will be connecting to
#define WEBROOT "./webroot" // the web server's root directory

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
#ifdef LIST
	struct Linked_List headers;
	init_list(&headers, 20);
#endif

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
			while(recv_line(sockfd, header)){
#ifdef LIST
				if(headers.size -2 == count){
					resize(&headers, count+10);
				}
#endif

				for(i = 0; i < strlen(header); i++){
//					printf("Header[%d]: %c\n", i, header[i]);
					if(header[i] == ": "[matched]){
						matched++;
//						printf("I:%d\n", i);
						if(matched == 2){
//							printf("%d\n", i);
							value[i-1] = '\0';
#ifdef NODE
							insertNew(value, &header[i + 1]);
#endif
#ifdef LIST
							strcpy(headers.keys[count], value);		//TODO: only use keys.value[]
							strcpy(headers.values[count], &header[i+1]);
#endif
							break;
						}
					}else{
						matched = 0;
					}
					value[i] = header[i];
				}
//				printf("I2:%d\n", i);
//				dump(header, strlen(header)+1);
//				dump(value, strlen(value)+1);
//				printf("%s\n", &header[i+1]);

				count++;
			}
#ifdef LIST
			printLinkedList(&headers);
#endif
#ifdef NODE
			printList();
#endif

//			contentLength = atoi(getValueByKey(&headers, "Content-Length"));
//			recv(sockfd, content, contentLength, 0);
//			dump(content, contentLength);
//			printf("Content-Length: %d\nContent: %s\n", contentLength, content);	//TODO fix double free or corruption(out)(free linked list?)
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
#ifdef NODE
	removeList();
#endif
#ifdef LIST
	end(&headers);
#endif
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
