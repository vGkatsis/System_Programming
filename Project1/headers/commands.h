#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "heap.h"
#include "linked_list.h"
#include "hashTable.h"
#include "patientRecords.h"
#include "error_handler.h"


//Reads Commands From User And Calls Command Handler
int readCommands(hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize);

//Handles Commands Given By User
int commandHandler(char *command,hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize);

//globalDiseaseStats command implementation
void globalDiseaseStats(hashTable *diseaseHashTable, int bucketSize);

//diseaseFrequency command implementation
void diseaseFrequency(hashTable *diseaseHashTable ,hashTable *countryHashTable, int bucketSize);

//top-kDiseases command implementation
void topkDiseases(hashTable *diseaseHashTable ,int bucketSize);

//top-kCountries command implementation
void topkCountries(hashTable *countryHashTable ,int bucketSize);

//insertPatientRecord command implementation
int insertPatientRecord(char *commandArguments,hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize);

//recordPatientExit command implementation
int recordPatientExit(hashTable *records, int bucektSize);

//numCurrentPatients command implementation
void numCurrentPatients(hashTable *diseaseHashTable,int bucketSize);
#endif