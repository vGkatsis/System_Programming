#ifndef STRING_HANDLER_H
#define STRING_HANDLER_H

#include <string.h>
#include <stdlib.h>
#include "../hdrs/linked_list.h"

//Concatenate two strings into a new one
void stringConcat(char **finalString, char *firstString, char *lastString);

//Add a slash '/' at the end of a string
void addSlashAtEnd(char **finalString);

#endif