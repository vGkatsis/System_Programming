#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include "communication.h"
#include "linked_list.h"
#include "hashTable.h"
#include "patientRecords.h"
#include "error_handler.h"
#include "fileReading.h"
#include "utilityFunctions.h"
#include "../hdrs/logFileStatistics.h"


//Server reads data from files
void fileReading(int clientFd, int serverFd,List **countriesList,hashTable *filePathsHashTable, hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize, logStats *logStatistics);

//Client Reads Commands From User And Writes To Named Pipe
char * clientReadCommands();

//Server Reads Commands From Named Pipe And Calls Command Handler
int serverReadCommand(char *command,int clientFd,int serverFd, List **countriesList,hashTable *filePathsHashTable, hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize, logStats *logStatistics);

//Handles Commands Given By User
int commandHandler(char *command,int clientFd, int serverFd,List **countriesList,hashTable *filePathsHashTable, hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize,logStats *logStatistics);

//listCountries command implementation
void listCountries(int serverFd,List *countriesList,logStats *logStatistics);

//diseaseFrequency command implementation
void diseaseFrequency(int serverFd,hashTable *diseaseHashTable ,hashTable *countryHashTable, int bucketSize,logStats *logStatistics);

//top-kAgeRanges command implementation
void topkAgeRanges(int serverFd,hashTable *countryHashTable ,int bucketSize,logStats *logStatistics);

//searchPatientRecord command implementation
void searchPatientRecord(int serverFd, hashTable *records, int bucketSize,logStats *logStatistics);

//numPatientAdmissions command implementation
void numPatientAdmissions(int serverFd, hashTable *countryHashTable,int bucketSize,logStats *logStatistics);

//numPatientDischarges command implementation
void numPatientDischarges(int serverFd, hashTable *countryHashTable,int bucketSize,logStats *logStatistics);

//send logStatistics back to aggregator
void sendLogStatistics(int serverFd, logStats *logStatistics);

//Function reading newly added files
void readNewFiles(hashTable *filePathsHashTable, hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize,logStats *logStatistics);
#endif