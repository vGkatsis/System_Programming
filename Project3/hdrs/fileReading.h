#ifndef FILE_READING_H
#define FILE_READING_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linked_list.h"
#include "hashTable.h"
#include "patientRecords.h"

//Function reading patient records file
int readPatientRecords(FILE *patientRecords, hashTable *records,hashTable *countryHashTable,
                                    hashTable *diseaseHashTable, int bucketSize, char *country, char *currentDate);

#endif