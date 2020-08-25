#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>	
#include <string.h>
#include "hashTable.h"

//Check If master program Has Been Caled With Correct Arguments
int argCheck(int argc, char** argv, int *numOfWorkers,int *bufferSize, char **serverIP, int *serverPort,char**inputDir);

//Check if whoServer program has been called with correct arguments
int serverArgCheck(int argc, char **argv, int *queryPortNum, int *statisticsPortNum, int *numOfThreads, int *bufferSize);

//Check if whoClient program has been called with correct arguments
int clientArgCheck(int argc, char **argv, FILE **queryFile, int *numThreads, int *serverPort, char **serverIP);

//Print Helpping Message
void printHelp();

//Print Correct Usage Message
void printUsage();

//Print Correct Usage Message For whoServer
void printServerUsage();

//Print Correct Usage Message For whoClient
void printClientUsage();

#endif