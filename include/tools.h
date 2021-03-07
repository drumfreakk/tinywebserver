#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A function to display an error message and then exit
void fatal(char *message);

// An error checked malloc() wrapper function
void *ec_malloc(unsigned int size);

// dumps raw memory in hex byte and printable split format
void dump(const unsigned char *data_buffer, const unsigned int length);

#endif
