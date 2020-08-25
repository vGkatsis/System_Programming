#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include "../hdrs/stringHandler.h"
#include "../hdrs/linked_list.h"

//Get the contents of a directory into a list
void listDirEntries(char *dirName, List **dirEntries);

#endif