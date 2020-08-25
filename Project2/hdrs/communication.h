#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../hdrs/hashTable.h"
#include "../hdrs/commands.h"
#include "../hdrs/logFileStatistics.h"
#include "../hdrs/commandResults.h"

//Read from a named pipe
void namedPipeRead(int Fd, char **result);

//Client writes to servers
void clientWriteToServer(int numOfWorkers, char *clientNamedPipePath, char *stringToWrite);

//Cliente distributes countrie dirs to servers
void serverDistributeCountries(List *countryDirList, int numOfWorkers, char *clientNamedPipePath, int singleWorker);

//Cliente reads from server
void clientReadFromServer(int numOfWorkers, struct pollfd *serverFdArray, int *serverFd, logStats *logStatistics);

//Server reads a command from client
void readCommand(int clientFd, int serverFd,List **countriesList,hashTable *filePathsHashTable,hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize,logStats *logStatistics);

//Write end of communication character
void endComm(int serverFd);

#endif