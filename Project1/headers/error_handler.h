#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>	
#include <string.h>
#include "hashTable.h"

//Check If Application Has Been Caled With Correct Number Of Arguments
int argCheck(int argc, char** argv, int *diseaseHashNum, int *countryHashNum, int *bucket_size, FILE **patientRecordsFile);

//Print Helpping Message
void printHelp();

//Print Correct Usage Message
void printUsage();

#endif