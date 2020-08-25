#include "../hdrs/hashTable.h"

//Initialize a bucketEntry struct
void initializeBucketEntry(bucketEntry **BucketEntry, char *key, void *value){

    if(((*BucketEntry) = (bucketEntry *)malloc(1 * sizeof(bucketEntry))) == NULL){
        perror("\nError while allocating space for bucket entry\n");
        exit(5);
    }

    if((((*BucketEntry)->key) = (char **)malloc(1 * sizeof(char *))) == NULL){
        perror("\nError while allocating space for bucketEntry key\n");
        exit(5);
    }

    (*((*BucketEntry)->key)) = key;
    (*BucketEntry)->value = value;
}

//Initialize a bucket struct
void initializeBucket(bucket **buck, int bucketSize){

    if(((*buck) = (bucket *)malloc(1 * sizeof(bucket))) == NULL){
        perror("\nError while allocating space for table bucket\n");
        exit(5);
    }

    int entriesNum = (bucketSize / (sizeof(bucketEntry)));

    (*buck)->entries = (bucketEntry **) malloc(entriesNum * sizeof(bucketEntry *));
    for(int i = 0; i < entriesNum; i++){
        (*buck)->entries[i] = NULL;
    }
    (*buck)->next = NULL;
}

//Initialize a hash table
void initializeHashTable(hashTable ** hashT, int tableSize){

    if(((*hashT) = (hashTable *) malloc(1 * sizeof(hashTable))) == NULL){
        perror("\nError while allocating space for hash table\n");
        exit(5);
    }

    (*hashT)->table = (bucket **)malloc(tableSize * sizeof(bucket*));
    for(int i = 0; i < tableSize; i++){
        (*hashT)->table[i] = NULL;
    }
    (*hashT)->tableSize = tableSize;
}

//Hash function for strings
int stringHash(hashTable *hashT, char * string){

    if(string == NULL){
        return -1;
    }

    __uint64_t hash = string[0];
    int p = 2;

    for(int i = 1; i < strlen(string); i++){
        
        if((i >= 20) && ((i % 2) == 0)){
            hash -= (string[i] * pow(p,i-10));    
            continue;
        }

        hash += (string[i] * pow(p,i));
    }

    return hash % hashT->tableSize;
}

//Insert a new element in the hash table
void insertRecordInHash(hashTable *hashT, char *hashString, patientRecord *pRecord, int bucketSize){

    if(hashT == NULL){
        perror("\nHash table hash not been created\n");
        exit(5);
    }

    //Get the hash value of the string id
    int hashValue = stringHash(hashT,hashString);

    if((hashValue < 0) || (hashValue >= hashT->tableSize)){
        perror("\nSomething gone wrong with string hashing. Hash value is not a valid one\n");
        exit(5);
    }

    //NUmber of entris each bucket can hold
    int entriesNum = (bucketSize / (sizeof(bucketEntry)));

    bucket *tmpBucket = hashT->table[hashValue];
    bucket *newBucket = tmpBucket;
    //If this bucket list is empty insert in a new bucket
    if(tmpBucket == NULL){
        initializeBucket(&newBucket,bucketSize);
        hashT->table[hashValue] = newBucket;
    }

    bucketEntry *tmpEntry;
    int entryIndex;
    //Find the first bucket containing an empty entry position for insertion
    while(newBucket != NULL){
        entryIndex = 0;
        while((entryIndex < entriesNum) && (newBucket->entries[entryIndex] != NULL)){
            
            if(strcmp((*newBucket->entries[entryIndex]->key),hashString) == 0){
                break;
            }
            entryIndex++;
        }
    
        if(entryIndex >= entriesNum){
            tmpBucket = newBucket;
            newBucket = newBucket->next;
        }else{
            break;
        }
    }

    //If no entry with given has string exists create a new one
    if((newBucket == NULL) || (newBucket->entries[entryIndex] == NULL)){

        avlTree *newAVLTree;
        date entryDate;

        initializeAvlTree(&newAVLTree);

        parseDate(pRecord->entryDate,&entryDate);
        insertNodeToAVL(newAVLTree,entryDate,pRecord);

        initializeBucketEntry(&tmpEntry,hashString,newAVLTree);
    }

    if(newBucket == NULL){
        initializeBucket(&newBucket,bucketSize);
        tmpBucket->next = newBucket;

        newBucket->entries[0] = tmpEntry;
    }else{

        if(newBucket->entries[entryIndex] == NULL){

            newBucket->entries[entryIndex] = tmpEntry;
        }else{

            avlTree *AVLTree = newBucket->entries[entryIndex]->value;
            
            date entryDate;           
            parseDate(pRecord->entryDate,&entryDate);
        
            insertNodeToAVL(AVLTree,entryDate,pRecord);
        }
    }
}

//Insert a new patientRecord element in the hash table
void insertElementInHash(hashTable *hashT, char *hashString, void *element, int bucketSize){

    if(hashT == NULL){
        perror("\nHash table hash not been created\n");
        exit(5);
    }

    int hashValue = stringHash(hashT,hashString);

    if((hashValue < 0) || (hashValue >= hashT->tableSize)){
        perror("\nSomething gone wrong with string hashing. Hash value is not a valid one\n");
        exit(5);
    }

    int entriesNum = (bucketSize / (sizeof(bucketEntry)));

    bucket *tmpBucket = hashT->table[hashValue];
    bucket *newBucket = tmpBucket;
    if(tmpBucket == NULL){
        initializeBucket(&newBucket,bucketSize);
        hashT->table[hashValue] = newBucket;
    }

    bucketEntry *tmpEntry;
    int entryIndex;
    while(newBucket != NULL){
        entryIndex = 0;    
        while((entryIndex < entriesNum) && (newBucket->entries[entryIndex] != NULL)){

            entryIndex++;
        }
    
        if(entryIndex >= entriesNum){
            tmpBucket = newBucket;
            newBucket = newBucket->next;
        }else{
            break;
        }
    }

    initializeBucketEntry(&tmpEntry,hashString,element);

    if(newBucket == NULL){
        initializeBucket(&newBucket,bucketSize);
        tmpBucket->next = newBucket;

        newBucket->entries[0] = tmpEntry;
    }else{

        newBucket->entries[entryIndex] = tmpEntry;
    }
    
}

//Find an element in th hash table
void *findRecordInHash(hashTable *hashT, char *elementKey, int bucketSize){

    int hashValue = stringHash(hashT,elementKey);

    bucket *currBucket = hashT->table[hashValue];

    if(currBucket == NULL){
        return NULL;
    }

    int numOfEntries = bucketSize / sizeof(bucketEntry);  
    bucketEntry *currEntry;

    //For each bucket in the bucket list
    while(currBucket != NULL){
        //For each entry in the bucket
        for(int j = 0; j < numOfEntries; j++){
            currEntry = currBucket->entries[j];
            //If entry exists
            if(currEntry != NULL){
                if(strcmp((*(currEntry->key)),elementKey) == 0){
                    return currEntry->value;
                }
            }
        }
        currBucket = currBucket->next;
    }

    return NULL;
}

char * getEntryKey(bucketEntry *BucketEntry){
    if(BucketEntry == NULL){
        return NULL;
    }

    return *(BucketEntry->key);
}
avlTree * getEntryValue(bucketEntry *bucketEntry){

    if(bucketEntry == NULL){
        return NULL;
    }

    return bucketEntry->value;
}

//Destroy a bucket entry
void freeBucketEntry(bucketEntry * bucketEntry,char *valueType){  

    //Free a bucket entry containing a tree
    if(strcmp(valueType,"tree") == 0){
        avlTree *valueTree;
        valueTree = (avlTree *)bucketEntry->value;
        
        freeAvlTree(valueTree->head);
        free(bucketEntry->key);
        free(bucketEntry->value);
    }
    //Free a bucket entry containing a patient record
    if(strcmp(valueType,"record") == 0){
        patientRecord *valueRecord = (patientRecord *)bucketEntry->value;
        
        freePatientRecord(valueRecord);
        free(bucketEntry->key);
    }
    //Free a bucket entry containing a list
    if(strcmp(valueType,"list") == 0){
        
        free((*bucketEntry->key));
        free(bucketEntry->key);
        freeList((List *)bucketEntry->value);
    }
    //Free a bucket entry containing a primitive data type
    if(strcmp(valueType,"primitive") == 0){
        
        free(bucketEntry->key);
    }
    free(bucketEntry);
}

//Destroy a bucket struct
void freeBucket(bucket *buck, int bucketSize,char *valueType){

    int entriesNum = (bucketSize / (sizeof(bucketEntry)));
    
    bucket *rmv;
    bucketEntry **entriesPtr;
    while(buck != NULL){
        entriesPtr = buck->entries;
        rmv = buck;
        for(int i = 0; i < entriesNum; i++){
            if(buck->entries[i] != NULL){
                freeBucketEntry(buck->entries[i],valueType);
            }
        }
        buck = buck->next;
        free(entriesPtr);
        free(rmv);
    }
}

//Destroy a hash table
void freeHashTable(hashTable *hashT,int bucketSize,char *valueType){

    for(int i = 0; i < hashT->tableSize; i++){
        if(hashT->table[i] != NULL){
            freeBucket(hashT->table[i],bucketSize,valueType);
        }
    }
    free(hashT->table);
    free(hashT);
}