#include "../headers/commands.h"

/*******************************************************
 *                                                     *
 *                  Command  Reader                    *
 *                                                     *
 *******************************************************/


//Reads Commands From User And Calls Command Handler
int readCommands(hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize){

    char *command, *cleanCommand;
    size_t minCommandSize = 6;

    if((command = (char *) malloc(minCommandSize * sizeof(char))) == NULL){
        perror("\nError While ALlocating Space For Command String\n");
        exit(1);
    }

//    printf("\nInsert command\n");
    
    getline(&command,&minCommandSize,stdin);
    while(strcmp(command,"\n") == 0){
        getline(&command,&minCommandSize,stdin);
    }
    

    //Clean given command from trash characters
    cleanCommand = strtok(command,"\n");
    if(cleanCommand[0] == '/'){
        memmove(cleanCommand, cleanCommand + 1, strlen(cleanCommand));
    }

    while(strcmp(cleanCommand,"exit") != 0){
        
        commandHandler(command,records, countryHashTable, diseaseHashTable,bucketSize);
        
//        printf("\nInsert command\n");
        getline(&command,&minCommandSize,stdin);
        while(strcmp(command,"\n") == 0){
            getline(&command,&minCommandSize,stdin);
        }

        //Clean given command from trash characters
        cleanCommand = strtok(command,"\n");
        if(cleanCommand[0] == '/'){
            memmove(cleanCommand, cleanCommand + 1, strlen(cleanCommand));
        }
    }

    free(command);
    return 1;
}


/*******************************************************
 *                                                     *
 *                  Command  Handler                   *
 *                                                     *
 *******************************************************/


int commandHandler(char *command,hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize){

    char *cleanCommand = strtok(command," ");

    //Call the function implementing the command given
    if(strcmp(cleanCommand,"globalDiseaseStats") == 0){
        globalDiseaseStats(diseaseHashTable,bucketSize);
    }else if(strcmp(cleanCommand,"diseaseFrequency") == 0){
        diseaseFrequency(diseaseHashTable, countryHashTable, bucketSize);
    }else if(strcmp(cleanCommand,"topk-Diseases") == 0){
        topkDiseases(diseaseHashTable,bucketSize);
    }else if(strcmp(cleanCommand,"topk-Countries") == 0){
        topkCountries(countryHashTable,bucketSize);
    }else if(strcmp(cleanCommand,"insertPatientRecord") == 0){
        if(!insertPatientRecord(command,records, countryHashTable, diseaseHashTable, bucketSize)){
            return 0;
        }
    }else if(strcmp(cleanCommand,"recordPatientExit") == 0){
        if(!recordPatientExit(records,bucketSize)){
            return 0;
        }
    }else if(strcmp(cleanCommand,"numCurrentPatients") == 0){
        numCurrentPatients(diseaseHashTable,bucketSize);
    }else{
        printHelp();
    }

}

/*******************************************************
 *                                                     *
 *            /globalDiseaseStats Command              *
 *                                                     *
 *******************************************************/


void globalDiseaseStats(hashTable *diseaseHashTable, int bucketSize){

    int numOfEntries = bucketSize / sizeof(bucketEntry);    
    char *option,*date1,*date2;
    date Date1, Date2;

    bucket *currBucket;
    bucketEntry *currEntry;
    avlTree * valueTree;

    //Case where no argument was given
    if((option = strtok(NULL, " ")) == NULL){    
        printf("\n");
        int hashTableSize = diseaseHashTable->tableSize;
        //Search each list of buckets of the diseaseHashTable
        for(int i = 0; i < hashTableSize; i++){
            //If the bucket list exists
            if(diseaseHashTable->table[i] != NULL){
                currBucket = diseaseHashTable->table[i];
                //For each bucket
                while(currBucket != NULL){
                    //For each entry in the bucket
                    for(int j = 0; j < numOfEntries; j++){
                        currEntry = currBucket->entries[j];
                        //If entry exists
                        if(currEntry != NULL){
                            valueTree = (avlTree *)currEntry->value;
                            //Print the name of the disease and the number of patients
                            printf("%s %d\n",(*currEntry->key),valueTree->numberOfEntries);
                        }
                    }
                    currBucket = currBucket->next;
                }
            }
        }
        return;
    }
    if((date1 = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for date1\n");
        exit(7);
    }
    strcpy(date1,option);

    //Check if second argument has been given
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nBoth date1 and date2 arguments must be provided.\n");
//        printf("Correct use of command is:\n");
//        printf("\tglobalDiseaseStats [date1 date2]\n");
        printf("error\n");
        free(date1);
        return;
    }
    if((date2 = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for date2\n");
        exit(7);
    }
    strcpy(date2,option);

    //Both arguments have been given
    parseDate(date1,&Date1);
    parseDate(date2,&Date2);

    //Check that date1 is previous than date2
    if(compareDates(&Date1,&Date2) > 0){
//        printf("\nExit Date can not be previous than Entry Date...\n");
        printf("error\n");
        free(date1);
        free(date2);
        return;
    }

//    printf("\n");
    int hashTableSize = diseaseHashTable->tableSize;
    //Search each list of buckets of the diseaseHashTable
    for(int i = 0; i < hashTableSize; i++){
        //If the bucket list exists
        if(diseaseHashTable->table[i] != NULL){
            currBucket = diseaseHashTable->table[i];
            //For each bucket
            while(currBucket != NULL){
                //For each entry in the bucket
                for(int j = 0; j < numOfEntries; j++){
                    currEntry = currBucket->entries[j];
                    //If entry exists
                    if(currEntry != NULL){
                        valueTree = (avlTree *)currEntry->value;
                        //Print name of disease and number of patients in the given date range
                        printf("%s %d\n",(*currEntry->key),dateRangeEntries(valueTree->head,&Date1,&Date2,"default",NULL));
                    }
                }
                currBucket = currBucket->next;
            }
        }
    }    
    free(date1);
    free(date2);
    return;
}


/*******************************************************
 *                                                     *
 *              /diseaseFrequency Command              *
 *                                                     *
 *******************************************************/



void diseaseFrequency(hashTable *diseaseHashTable, hashTable *countryHashTable, int bucketSize){
    
    int numOfEntries = bucketSize / sizeof(bucketEntry);  
    char *option,*diseaseId, *date1, *date2, *country;

    date Date1, Date2;

    bucket *currBucket, *tmpBucket;
    bucketEntry *currEntry, *tmpEntry;
    avlTree *valueTree;

    //Gathere all given arguments
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\tdiseaseFrequency <diseaseId> <date1> <date2> [country]\n");
        printf("error\n");
        return;
    }
    if((diseaseId = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for diseaseId\n");
        exit(7);
    }
    strcpy(diseaseId,option);

    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\tdiseaseFrequency <diseaseId> <date1> <date2> [country]\n");
        printf("error\n");
        free(diseaseId);
        return;
    }
    if((date1 = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for date1\n");
        exit(7);
    }
    strcpy(date1,option);

    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\tdiseaseFrequency <diseaseId> <date1> <date2> [country]\n");
        printf("error\n");
        free(diseaseId);
        free(date1);
        return;
    }
    if((date2 = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for date2\n");
        exit(7);
    }
    strcpy(date2,option);

    //If no country name was given
    if((option = strtok(NULL, " ")) == NULL){      
        
        //Both arguments have been given
        parseDate(date1,&Date1);
        parseDate(date2,&Date2);

        //Check that date1 is previous than date2
        if(compareDates(&Date1,&Date2) > 0){
//            printf("\nExit Date can not be previous than Entry Date...\n");
            printf("error\n");
            free(diseaseId);
            free(date1);
            free(date2);
            return;
        }

//        printf("\n");
        int hashTableSize = diseaseHashTable->tableSize;
        //Find the bucket list containing the given diseaseId
        currBucket = diseaseHashTable->table[stringHash(diseaseHashTable,diseaseId)];
        //If the bucket list exists
        if(currBucket != NULL){
            //For each bucket
            while(currBucket != NULL){
                //For each entry in the bucket
                for(int j = 0; j < numOfEntries; j++){
                    currEntry = currBucket->entries[j];
                    //If entry exists
                    if(currEntry != NULL){
                        if(strcmp((*(currEntry->key)),diseaseId) == 0){
                            valueTree = (avlTree *)currEntry->value;
                            //Print the name fo the disease and the number of patients in the given date range
                            printf("%s %d\n",(*currEntry->key),dateRangeEntries(valueTree->head,&Date1,&Date2,"default",NULL));

                            free(diseaseId);
                            free(date1);
                            free(date2);
                            return;
                        }
                    }
                }
                currBucket = currBucket->next;
            }
        }
        printf("%s 0\n",diseaseId);
        free(diseaseId);
        free(date1);
        free(date2);

        return;
    }
    if((country = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for date2\n");
        exit(7);
    }
    strcpy(country,option);

    //Both arguments have been given
    parseDate(date1,&Date1);
    parseDate(date2,&Date2);

    //Check that date1 is previous than date2
    if(compareDates(&Date1,&Date2) > 0){
//        printf("\nExit Date can not be previous than Entry Date...\n");
        printf("error\n");
        free(diseaseId);
        free(country);
        free(date1);
        free(date2);
        return;
    }
    //If a country name has been provided:
    
    //Since the desired number of patients can be found both by diseaseHashTable and countryHashTabl
    currBucket = diseaseHashTable->table[stringHash(diseaseHashTable,diseaseId)];
    tmpBucket = countryHashTable->table[stringHash(countryHashTable,country)];

    int foundEntry;
    int diseaseTreeEntries = 0;
    int countryTreeEntries = 0;

    //Find the given disease in the diseasehashTable
    foundEntry = 0;
    if(currBucket != NULL){
        //For each bucket
        while(currBucket != NULL){
            //For each entry in the bucket
            for(int j = 0; j < numOfEntries; j++){
                currEntry = currBucket->entries[j];
                //If entry exists
                if(currEntry != NULL){
                    if(strcmp((*(currEntry->key)),diseaseId) == 0){
                        valueTree = (avlTree *)currEntry->value;
                        diseaseTreeEntries = valueTree->numberOfEntries;
                        foundEntry = 1;
                        break;
                    }
                }
            }
            if(foundEntry){
                break;
            }
            currBucket = currBucket->next;
        }
    }

    //Find the given country in the countryHashTable
    foundEntry = 0;
    if(tmpBucket != NULL){
        //For each bucket
        while(tmpBucket != NULL){
            //For each entry in the bucket
            for(int j = 0; j < numOfEntries; j++){
                tmpEntry = tmpBucket->entries[j];
                //If entry exists
                if(tmpEntry != NULL){
                    if(strcmp((*(tmpEntry->key)),country) == 0){
                        valueTree = (avlTree *)tmpEntry->value;
                        countryTreeEntries = valueTree->numberOfEntries;
                        foundEntry = 1;
                        break;
                    }
                }
            }
            if(foundEntry){
                break;
            }
            tmpBucket = tmpBucket->next;
        }
    }

    //Compare number of nodes in each avlTree and chose the one with the smallest
    if(diseaseTreeEntries <= countryTreeEntries){
        if(currEntry != NULL){
            valueTree = (avlTree *)currEntry->value;
            printf("%s %d\n",diseaseId,dateRangeEntries(valueTree->head,&Date1,&Date2,"country",country));
        }else{
            printf("%s 0\n",diseaseId);
        }
    }else{
        if(tmpEntry != NULL){
            valueTree = (avlTree *)tmpEntry->value;
            printf("%s %d\n",diseaseId,dateRangeEntries(valueTree->head,&Date1,&Date2,"disease",diseaseId));
        }else{
            printf("%s 0\n",diseaseId);
        }
    }

    free(diseaseId);
    free(country);
    free(date1);
    free(date2);
}



/*******************************************************
 *                                                     *
 *              /top-kDiseases Command                 *
 *                                                     *
 *******************************************************/



void topkDiseases(hashTable *diseaseHashTable ,int bucketSize){

    int k;
    int numOfEntries = bucketSize / sizeof(bucketEntry);  
    char *option,*country, *date1, *date2;
    date Date1, Date2;

    bucket *diseaseBucket,*countryBucket;
    bucketEntry *diseaseEntry, *countryEntry;
    avlTree *valueTree;
    hashTable *kDiseaseHashTable;

    //heap *maxHeap;
    //bucket *heapBucket;
    //bucketEntry *heapBucketEntry;

    //Gathere all given arguments
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\t/topk-Countries k disease [date1 date2]\n");
        printf("error\n");
        return;
    }
    k = atoi(option);
    
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\t/topk-Countries k disease [date1 date2]\n");
        printf("error\n");
        return;
    }
    if((country = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for diseaseId\n");
        exit(7);
    }
    strcpy(country,option);

    //If no date range has been given
    if((option = strtok(NULL, " ")) == NULL){
        
        date zeroDate,infDate;

        zeroDate.day = 0;
        zeroDate.month = 0;
        zeroDate.year = 0;

        infDate.day = 99;
        infDate.month = 99;
        infDate.year = 9999;



        // CREATION OF MAX-HEAP
        //initializeHeap(&maxHeap);

        //Create a new hash table to keep number of patients for each disease
        initializeHashTable(&kDiseaseHashTable,diseaseHashTable->tableSize);

        int diseaseHashTableSize = diseaseHashTable->tableSize;
        //Search each list of buckets of the diseaseHashTable
        for(int i = 0; i < diseaseHashTableSize; i++){
            //If the bucket list exists
            if(diseaseHashTable->table[i] != NULL){
                diseaseBucket = diseaseHashTable->table[i];
                //For each bucket
                while(diseaseBucket != NULL){
                    //For each entry in the bucket
                    for(int j = 0; j < numOfEntries; j++){
                        diseaseEntry = diseaseBucket->entries[j];
                        //If entry exists
                        if(diseaseEntry != NULL){
                            valueTree = (avlTree *)diseaseEntry->value;

                            //Insert the name of the disease and the number of patients
                            int *diseaseNum = (int *)malloc(1 * sizeof(int));
                            *diseaseNum = dateRangeEntries(valueTree->head,&zeroDate,&infDate,"country",country);
                            insertElementInHash(kDiseaseHashTable,(*(diseaseEntry->key)),diseaseNum,sizeof(bucketEntry));
                        
                            //      INSERTION OF ELEMENTS IN MAX HEAP
                            // heapBucket = kDiseaseHashTable->table[stringHash(kDiseaseHashTable,(*(diseaseEntry->key)))];
                            // heapBucketEntry = heapBucket->entries[0];
                            // while(heapBucket != NULL){
                            //     if(strcmp((*(heapBucketEntry->key)),(*(diseaseEntry->key))) == 0){
                            //         break;
                            //     }
                            //     heapBucket = heapBucket->next;
                            //     heapBucketEntry = heapBucket->entries[0];
                            // }
                            // insertHeapNode(maxHeap,heapBucketEntry);
                        }
                    }
                    diseaseBucket = diseaseBucket->next;
                }
            }
        }    

        // PRINT TOP-K RESULTS FROM MAX-HEAP
        // int *heapValue;
        // bucketEntry *heapExtractEntry;
        // if(maxHeap->numberOfNodes < k){
        //     k = maxHeap->numberOfNodes;
        // }
        // for(int i = 0; i < k; i++){
        //     heapExtractEntry = removeRootNode(maxHeap);
        //     if(heapExtractEntry != NULL){
        //         heapValue = (int *)heapExtractEntry->value;
        //         if((*heapValue) != 0){
        //             printf("%s %d\n",(*(heapExtractEntry->key)),(*(heapValue)));
        //         }else{
        //             break;
        //         }
        //     }else{
        //         break;
        //     }
        // }


        //freeHeap(maxHeap->head);
        //free(maxHeap);
        free(country);
        freeHashTable(kDiseaseHashTable,sizeof(bucketEntry),"number");
        return;
    }
    if((date1 = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for date1\n");
        exit(7);
    }
    strcpy(date1,option);

    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nSince date1 has been given a date2 is mandatory.\n");
        printf("error\n");
        free(country);
        free(date1);
        return;
    }
    if((date2 = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for date2\n");
        exit(7);
    }
    strcpy(date2,option);     
        
    //Both arguments have been given
    parseDate(date1,&Date1);
    parseDate(date2,&Date2);

    //Check that date1 is previous than date2
    if(compareDates(&Date1,&Date2) > 0){
//      printf("\nExit Date can not be previous than Entry Date...\n");
        printf("error\n");
        free(country);
        free(date1);
        free(date2);
        return;
    }


    //If a date range has been provided.
    initializeHashTable(&kDiseaseHashTable,diseaseHashTable->tableSize);

        int diseaseHashTableSize = diseaseHashTable->tableSize;
        //Search each list of buckets of the diseaseHashTable
        for(int i = 0; i < diseaseHashTableSize; i++){
            //If the bucket list exists
            if(diseaseHashTable->table[i] != NULL){
                diseaseBucket = diseaseHashTable->table[i];
                //For each bucket
                while(diseaseBucket != NULL){
                    //For each entry in the bucket
                    for(int j = 0; j < numOfEntries; j++){
                        diseaseEntry = diseaseBucket->entries[j];
                        //If entry exists
                        if(diseaseEntry != NULL){
                            valueTree = (avlTree *)diseaseEntry->value;
                            
                            //Insert the name of the disease and the number of patients
                            int *diseaseNum = (int *)malloc(1 * sizeof(int));
                            *diseaseNum = dateRangeEntries(valueTree->head,&Date1,&Date2,"country",country);
                            insertElementInHash(kDiseaseHashTable,(*(diseaseEntry->key)),diseaseNum,sizeof(bucketEntry));
                        
                            //      INSERTION OF ELEMENTS IN MAX HEAP
                            // heapBucket = kDiseaseHashTable->table[stringHash(kDiseaseHashTable,(*(diseaseEntry->key)))];
                            // heapBucketEntry = heapBucket->entries[0];
                            // while(heapBucket != NULL){
                            //     if(strcmp((*(heapBucketEntry->key)),(*(diseaseEntry->key))) == 0){
                            //         break;
                            //     }
                            //     heapBucket = heapBucket->next;
                            //     heapBucketEntry = heapBucket->entries[0];
                            // }
                            // insertHeapNode(maxHeap,heapBucketEntry);
                        }
                    }
                    diseaseBucket = diseaseBucket->next;
                }
            }
        }


    // PRINT TOP-K RESULTS FROM MAX-HEAP
    // int *heapValue;
    // bucketEntry *heapExtractEntry;
    // if(maxHeap->numberOfNodes < k){
    //     k = maxHeap->numberOfNodes;
    // }
    // for(int i = 0; i < k; i++){
    //     heapExtractEntry = removeRootNode(maxHeap);
    //     if(heapExtractEntry != NULL){
    //         heapValue = (int *)heapExtractEntry->value;
    //         if((*heapValue) != 0){
    //             printf("%s %d\n",(*(heapExtractEntry->key)),(*(heapValue)));
    //         }else{
    //             break;
    //         }
    //     }else{
    //         break;
    //     }
    // }


    //freeHeap(maxHeap->head);
    //free(maxHeap);
    free(country);
    free(date1);
    free(date2);
    freeHashTable(kDiseaseHashTable,sizeof(bucketEntry),"number");
    return;
}


/*******************************************************
 *                                                     *
 *              /top-kCountries Command                *
 *                                                     *
 *******************************************************/



void topkCountries(hashTable *countryHashTable ,int bucketSize){

    int k;
    int numOfEntries = bucketSize / sizeof(bucketEntry);  
    char *option,*diseaseId, *date1, *date2;

    date Date1, Date2;
    bucket *countryBucket;
    bucketEntry *countryEntry;
    hashTable *kCountryHashTable;

    //heap *maxHeap;
    //bucket *heapBucket;
    //bucketEntry *heapBucketEntry;

    avlTree *valueTree;

    //Gathere all given arguments
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\t/topk-Countries k disease [date1 date2]\n");
        printf("error\n");
        return;
    }
    k = atoi(option);
    
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\t/topk-Countries k disease [date1 date2]\n");
        printf("error\n");
        return;
    }
    if((diseaseId = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for diseaseId\n");
        exit(7);
    }
    strcpy(diseaseId,option);

    //If no date range has been given
    if((option = strtok(NULL, " ")) == NULL){
        
        date zeroDate,infDate;

        zeroDate.day = 0;
        zeroDate.month = 0;
        zeroDate.year = 0;

        infDate.day = 99;
        infDate.month = 99;
        infDate.year = 9999;

        // CREATION OF MAX-HEAP
        //initializeHeap(&maxHeap);

        //Create a new hash table to hold each country and the number of patients
        initializeHashTable(&kCountryHashTable,countryHashTable->tableSize);

        int countryHashTableSize = countryHashTable->tableSize;
        //Search each list of buckets of the diseaseHashTable
        for(int i = 0; i < countryHashTableSize; i++){
            //If the bucket list exists
            if(countryHashTable->table[i] != NULL){
                countryBucket = countryHashTable->table[i];
                //For each bucket
                while(countryBucket != NULL){
                    //For each entry in the bucket
                    for(int j = 0; j < numOfEntries; j++){
                        countryEntry = countryBucket->entries[j];
                        //If entry exists
                        if(countryEntry != NULL){
                            valueTree = (avlTree *)countryEntry->value;

                            //Store country name and number of patients
                            int *diseaseNum = (int *)malloc(1 * sizeof(int));
                            *diseaseNum = dateRangeEntries(valueTree->head,&zeroDate,&infDate,"disease",diseaseId);
                            insertElementInHash(kCountryHashTable,(*(countryEntry->key)),diseaseNum,sizeof(bucketEntry));

                            //      INSERTION OF ELEMENTS IN MAX HEAP
                            // heapBucket = kCountryHashTable->table[stringHash(kCountryHashTable,(*(countryEntry->key)))];
                            // heapBucketEntry = heapBucket->entries[0];
                            // while(heapBucket != NULL){
                            //     if(strcmp((*(heapBucketEntry->key)),(*(countryEntry->key))) == 0){
                            //         break;
                            //     }
                            //     heapBucket = heapBucket->next;
                            //     heapBucketEntry = heapBucket->entries[0];
                            // }
                            // insertHeapNode(maxHeap,heapBucketEntry);
                        }
                    }
                    countryBucket = countryBucket->next;
                }
            }
        }    

        // PRINT TOP-K RESULTS FROM MAX-HEAP
        // int *heapValue;
        // bucketEntry *heapExtractEntry;
        // if(maxHeap->numberOfNodes < k){
        //     k = maxHeap->numberOfNodes;
        // }
        // for(int i = 0; i < k; i++){
        //     heapExtractEntry = removeRootNode(maxHeap);
        //     if(heapExtractEntry != NULL){
        //         heapValue = (int *)heapExtractEntry->value;
        //         if((*heapValue) != 0){
        //             printf("%s %d\n",(*(heapExtractEntry->key)),(*(heapValue)));
        //         }else{
        //             break;
        //         }
        //     }else{
        //         break;
        //     }
        // }


        free(diseaseId);
        //freeHeap(maxHeap->head);
        //free(maxHeap);
        freeHashTable(kCountryHashTable,sizeof(bucketEntry),"number");
        return;
    }
    if((date1 = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for date1\n");
        exit(7);
    }
    strcpy(date1,option);

    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nSince date1 has been given a date2 is mandatory.\n");
        printf("error\n");
        free(diseaseId);
        free(date1);
        return;
    }
    if((date2 = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for date2\n");
        exit(7);
    }
    strcpy(date2,option);     
        
    //Both arguments have been given
    parseDate(date1,&Date1);
    parseDate(date2,&Date2);

    //Check that date1 is previous than date2
    if(compareDates(&Date1,&Date2) > 0){
//      printf("\nExit Date can not be previous than Entry Date...\n");
        printf("error\n");
        free(diseaseId);
        free(date1);
        free(date2);
        return;
    }

    //If a date range has been given

    // CREATION OF MAX-HEAP
    //initializeHeap(&maxHeap);

    initializeHashTable(&kCountryHashTable,countryHashTable->tableSize);

        int countryHashTableSize = countryHashTable->tableSize;
        //Search each list of buckets of the diseaseHashTable
        for(int i = 0; i < countryHashTableSize; i++){
            //If the bucket list exists
            if(countryHashTable->table[i] != NULL){
                countryBucket = countryHashTable->table[i];
                //For each bucket
                while(countryBucket != NULL){
                    //For each entry in the bucket
                    for(int j = 0; j < numOfEntries; j++){
                        countryEntry = countryBucket->entries[j];
                        //If entry exists
                        if(countryEntry != NULL){
                            valueTree = (avlTree *)countryEntry->value;
                            
                            //Insert the name of the country and the number of patients
                            int *countryNum = (int *)malloc(1 * sizeof(int));
                            *countryNum = dateRangeEntries(valueTree->head,&Date1,&Date2,"disease",diseaseId);
                            insertElementInHash(kCountryHashTable,(*(countryEntry->key)),countryNum,sizeof(bucketEntry));
                        
                            //      INSERTION OF ELEMENTS IN MAX HEAP
                            // heapBucket = kCountryHashTable->table[stringHash(kCountryHashTable,(*(countryEntry->key)))];
                            // heapBucketEntry = heapBucket->entries[0];
                            // while(heapBucket != NULL){
                            //     if(strcmp((*(heapBucketEntry->key)),(*(countryEntry->key))) == 0){
                            //         break;
                            //     }
                            //     heapBucket = heapBucket->next;
                            //     heapBucketEntry = heapBucket->entries[0];
                            // }
                            // insertHeapNode(maxHeap,heapBucketEntry);
                        }
                    }
                    countryBucket = countryBucket->next;
                }
            }
        }

    // PRINT TOP-K RESULTS FROM MAX-HEAP
    // int *heapValue;
    // bucketEntry *heapExtractEntry;
    // if(maxHeap->numberOfNodes < k){
    //     k = maxHeap->numberOfNodes;
    // }
    // for(int i = 0; i < k; i++){
    //     heapExtractEntry = removeRootNode(maxHeap);
    //     if(heapExtractEntry != NULL){
    //         heapValue = (int *)heapExtractEntry->value;
    //         if((*heapValue) != 0){
    //             printf("%s %d\n",(*(heapExtractEntry->key)),(*(heapValue)));
    //         }else{
    //             break;
    //         }
    //     }else{
    //         break;
    //     }
    // }


    free(diseaseId);
    //freeHeap(maxHeap->head);
    //free(maxHeap);
    free(date1);
    free(date2);
    freeHashTable(kCountryHashTable,sizeof(bucketEntry),"number");
    return;
}



/*******************************************************
 *                                                     *
 *           /insertPatientRecord Command              *
 *                                                     *
 *******************************************************/



int insertPatientRecord(char *commandArguments,hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize){
    
    char *option,*value;
    patientRecord *pRecord;

    //Initialize a patient record struct
    initializePatientRecord(&pRecord);

    //Gathere all given information
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\tinsertPatientRecord <recordID> <patientFirstName> <patientLastName> <diseaseID> <country> <entryDate> [exitDate]\n");
//        printf("\tRecord rejected\n");
        printf("error\n");
        freePatientRecord(pRecord);
        return 0;
    }
    if((value = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for recordId\n");
        exit(7);
    }
    strcpy(value,option);
    setRecordId(pRecord,value);

    if(findRecordInHash(records,value,bucketSize) != NULL){
//        printf("\nRecord with recordId \"%s\" is a duplicate entry...\n",value);
//        printf("Ending application\n");
        printf("error\n");
        freePatientRecord(pRecord);
        return 0;
    }
    
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\tinsertPatientRecord <recordID> <patientFirstName> <patientLastName> <diseaseID> <country> <entryDate> [exitDate]\n");
//        printf("\tRecord rejected\n");
        printf("error\n");
        freePatientRecord(pRecord);
        return 0;
    }
    if((value = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for FirstName\n");
        exit(7);
    }
    strcpy(value,option);
    setFirstName(pRecord,value);
    
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\tinsertPatientRecord <recordID> <patientFirstName> <patientLastName> <diseaseID> <country> <entryDate> [exitDate]\n");
//        printf("\tRecord rejected\n");
        printf("error\n");
        freePatientRecord(pRecord);
        return 0;
    }
    if((value = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for LastName\n");
        exit(7);
    }
    strcpy(value,option);
    setLastName(pRecord,value);
    
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\tinsertPatientRecord <recordID> <patientFirstName> <patientLastName> <diseaseID> <country> <entryDate> [exitDate]\n");
//        printf("\tRecord rejected\n");
        printf("error\n");
        freePatientRecord(pRecord);
        return 0;
    }
    if((value = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for DiseaseID\n");
        exit(7);
    }
    strcpy(value,option);
    setDiseaseID(pRecord,value);
    
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\tinsertPatientRecord <recordID> <patientFirstName> <patientLastName> <diseaseID> <country> <entryDate> [exitDate]\n");
//        printf("\tRecord rejected\n");
        printf("error\n");
        freePatientRecord(pRecord);
        return 0;
    }
    if((value = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for Country\n");
        exit(7);
    }
    strcpy(value,option);
    setCountry(pRecord,value);
    
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\tinsertPatientRecord <recordID> <patientFirstName> <patientLastName> <diseaseID> <country> <entryDate> [exitDate]\n");
//        printf("\tRecord rejected\n");
        printf("error\n");
        freePatientRecord(pRecord);
        return 0;
    }
    if((value = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for Entry Date\n");
        exit(7);
    }
    strcpy(value,option);
    setEntryDate(pRecord,value);
    
    if((option = strtok(NULL, " ")) == NULL){
        if((value = (char *) malloc(2 * sizeof(char))) == NULL){
            perror("\nError while allocating space for ExitDate\n");
            exit(7);
        }
    
        strcpy(value,"-");
        setExitDate(pRecord,value);
    }else{
        if((value = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
            perror("\nError while allocating space for ExitDate\n");
            exit(7);
        }
        strcpy(value,option);
        setExitDate(pRecord,value);
    }

    
    insertElementInHash(records,pRecord->recordId,pRecord,bucketSize);
    //Insert patientRecord in country hashTable
    insertRecordInHash(countryHashTable,pRecord->Country,pRecord,bucketSize);
    //Insert patientRecord in disease hashTable
    insertRecordInHash(diseaseHashTable,pRecord->diseaseId,pRecord,bucketSize);
    printf("Record added\n");

    return 1;
}



/*******************************************************
 *                                                     *
 *             /recordPatientExit Command              *
 *                                                     *
 *******************************************************/


int recordPatientExit(hashTable *records, int bucketSize){
    
    char *option,*recordId,*exitDate;
    patientRecord *pRecord;
    date Entry, Exit;

    //Gathere all given information
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\trecordPatientExit <recordID> <exitDate>\n");
//        printf("\tRecord change rejected\n");
        printf("error\n");
        return 0;
    }
    if((recordId = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for recordId\n");
        exit(7);
    }
    strcpy(recordId,option);

    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\trecordPatientExit <recordID> <exitDate>\n");
//        printf("\tRecord change rejected\n");
        printf("error\n");
        free(recordId);
        return 0;
    }
    if((exitDate = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for exit date\n");
        exit(7);
    }
    strcpy(exitDate,option);

    
    if((pRecord = findRecordInHash(records,recordId,bucketSize)) != NULL){
        
        parseDate(pRecord->entryDate,&Entry);
        parseDate(exitDate,&Exit);

        if(compareDates(&Entry,&Exit) > 0){
//            printf("\nExit Date can not be previous than Entry Date...\n");
//            printf("Record change rejected\n");
            printf("error\n");
            free(recordId);
            free(exitDate);
            return 0;
        }

        free(pRecord->exitDate);
        setExitDate(pRecord,exitDate);

    }else{
//        printf("\nRecord with recordId: \"%s\" does not exist\n",recordId);
//        printf("Record change rejected\n");
        printf("Not found\n");
        free(recordId);
        free(exitDate);
        return 0;
    }

    printf("Record updated\n");
    free(recordId);
    return 1;
}


/*******************************************************
 *                                                     *
 *              /numCurrentPatients Command            *
 *                                                     *
 *******************************************************/


void numCurrentPatients(hashTable *diseaseHashTable, int bucketSize){
    
    int currentPatientsNum;
    char *option,*diseaseId;
    bucketEntry *currEntry;
    bucket *currBucket;
    avlTree *valueTree;

    int numOfEntries = bucketSize / sizeof(bucketEntry);

    printf("\n");
    //Case where disease argument was not given
    if((option = strtok(NULL, " ")) == NULL){
        int hashTableSize = diseaseHashTable->tableSize;
        //Search each list of buckets of the diseaseHashTable
        for(int i = 0; i < hashTableSize; i++){
            //If the bucket list exists
            if(diseaseHashTable->table[i] != NULL){
                currBucket = diseaseHashTable->table[i];
                //For each bucket
                while(currBucket != NULL){
                    //Foe each entry in the bucket
                    for(int j = 0; j < numOfEntries; j++){
                        currEntry = currBucket->entries[j];
                        //If entry exists
                        if(currEntry != NULL){
                            valueTree = (avlTree *)currEntry->value;

                            printf("%s %d\n",(*currEntry->key),currentPatients(valueTree->head));
                        }
                    }
                    currBucket = currBucket->next;
                }
            }
        }
        return;
    }
    //If a diseaseId has been inserted
    if((diseaseId = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
//        perror("\nError while allocating space for diseaseId\n");
        printf("error\n");
        exit(7);
    }
    strcpy(diseaseId,option);

    int hashTableSize = diseaseHashTable->tableSize;
    int diseaseHash = stringHash(diseaseHashTable,diseaseId);
    //If the bucket list exists
    if(diseaseHashTable->table[diseaseHash] != NULL){
        currBucket = diseaseHashTable->table[diseaseHash];
        //For each bucket
        while(currBucket != NULL){
            //Foe each entry in the bucket
            for(int j = 0; j < numOfEntries; j++){
                currEntry = currBucket->entries[j];
                //If entry exists
                if(currEntry != NULL){
                    //Check if this is the entry of the diseaseId given
                    if(strcmp((*(currEntry->key)),diseaseId) == 0){
                        valueTree = (avlTree *)currEntry->value;
                        printf("%s %d\n",(*currEntry->key),currentPatients(valueTree->head));
                        free(diseaseId);
                        return;
                    }
                }
            }
            currBucket = currBucket->next;
        }
    }
//    printf("\nNo patient record with diseasId \"%s\" exists.\n",diseaseId);
    printf("%s 0\n",diseaseId);
    free(diseaseId);
    return;
}