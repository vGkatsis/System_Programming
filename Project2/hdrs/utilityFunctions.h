#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include "../hdrs/stringHandler.h"
#include "../hdrs/linked_list.h"
#include "../hdrs/logFileStatistics.h"

//Get the contents of a directory into a list
void listDirEntries(char *dirName, List **dirEntries);

//Create a log file with data contained in logFileStatistics
void createLogFile(logStats *logFileStatistics);

#endif