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

//Server reads data from files
void fileReading(int clientFd, int serverFd,List **countriesList,hashTable *filePathsHashTable, hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize);

//Client Reads Commands From User And Writes To Named Pipe
char * clientReadCommands();

//Server Reads Commands From Named Pipe And Calls Command Handler
int serverReadCommand(char *command,int clientFd,int serverFd, List **countriesList,hashTable *filePathsHashTable, hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize);

//Handles Commands Given By User
int commandHandler(char *command,int clientFd, int serverFd,List **countriesList,hashTable *filePathsHashTable, hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize);

//diseaseFrequency command implementation
void diseaseFrequency(int serverFd,hashTable *diseaseHashTable ,hashTable *countryHashTable, int bucketSize);

//top-kAgeRanges command implementation
void topkAgeRanges(int serverFd,hashTable *countryHashTable ,int bucketSize);

//searchPatientRecord command implementation
void searchPatientRecord(int serverFd, hashTable *records, int bucketSize);

//numPatientAdmissions command implementation
void numPatientAdmissions(int serverFd, hashTable *countryHashTable,int bucketSize);

//numPatientDischarges command implementation
void numPatientDischarges(int serverFd, hashTable *countryHashTable,int bucketSize);

//send logStatistics back to aggregator
void sendLogStatistics(int serverFd);

//Function reading newly added files
void readNewFiles(hashTable *filePathsHashTable, hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize);
#endif