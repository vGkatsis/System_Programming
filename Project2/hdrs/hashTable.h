#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "avlTree.h"
#include "linked_list.h"

typedef struct Bucket_Entry{

    char **key;
    void *value;
}bucketEntry;

typedef struct Bucket{

    bucketEntry **entries;
    struct Bucket *next;
}bucket;

typedef struct Hash_Table{

    int tableSize;
    bucket **table;
}hashTable;

/*************************************
 *                                   *
 *     Initialization/Destruction    *
 *                                   *
 *************************************/
//Initialize a bucketEntry struct with an avlTree as value
void initializeBucketEntry(bucketEntry **BucketEntry, char *key, void *value);

//Initialize a bucket struct
void initializeBucket(bucket **buck, int bucketSize);

//Initialize a hash table
void initializeHashTable(hashTable ** hashT, int tableSize);

//Destroy a bucket entry
void freeBucketEntry(bucketEntry * bucketEntry, char *valueType);

//Destroy a bucket struct
void freeBucket(bucket *buck, int bucketSize, char *valueType);

//Destroy a hash table
void freeHashTable(hashTable *hashT, int bucketSize, char *valueType);

/*************************************
 *                                   *
 *              Utilities            *
 *                                   *
 *************************************/
//Hash function for strings
int stringHash(hashTable *hashT, char * string);

//Insert a new element in the hash table
void insertRecordInHash(hashTable *hashT, char *hashString, patientRecord *pRecord, int bucketSize);

//Insert a new patientRecord element in the hash table
void insertElementInHash(hashTable *hashT, char *hashString, void *element, int bucketSize);

//Find an element in the hash table
void *findRecordInHash(hashTable *hashT, char *elementKey, int bucketSize);

/**************************************
 *                                    *
 *          Getters/Setters           *
 *                                    *
 **************************************/

char * getEntryKey(bucketEntry *BucketEntry);
avlTree * getEntryValue(bucketEntry *bucketEntry);
#endif