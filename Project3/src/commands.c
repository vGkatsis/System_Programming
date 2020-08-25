 #include "../hdrs/commands.h"

/*******************************************************
 *                                                     *
 *               Client Command  Reader                *
 *                                                     *
 *******************************************************/

//Client Reads Commands From User And Writes To Named Pipe
char *clientReadCommands(){

    char *command, *tmpCommand,*cleanCommand;
    size_t minCommandSize = 6;   

    if((command = (char *) malloc(minCommandSize * sizeof(char))) == NULL){
        perror("\nError While ALlocating Space For Command String\n");
        exit(1);
    }

    printf("\nInsert command\n");
    
    getline(&command,&minCommandSize,stdin);
    while(strcmp(command,"\n") == 0){
        getline(&command,&minCommandSize,stdin);
    }

    tmpCommand = strtok(command,"\n");

    if((cleanCommand = (char *)malloc((strlen(tmpCommand) + 1) * sizeof(char))) == NULL){
        perror("Error while allocating space for clean command\n");
        exit(7);
    }
    strcpy(cleanCommand,tmpCommand);

    free(command);

    return cleanCommand;
}


/*******************************************************
 *                                                     *
 *               Server  Command  Reader               *
 *                                                     *
 *******************************************************/


//Server reads command and forwards to command handler
int serverReadCommand(char *command,int clientFd, int serverFd,List **countriesList,hashTable *filePathsHashTable, hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize){

    //Clean given command from trash characters
    if(command[0] == '/'){
        memmove(command, command + 1, strlen(command));
    }

    commandHandler(command,clientFd, serverFd,countriesList,filePathsHashTable,records, countryHashTable, diseaseHashTable,bucketSize);

    return 1;
}


/*******************************************************
 *                                                     *
 *                  Command  Handler                   *
 *                                                     *
 *******************************************************/

//Handles Commands Given By User
int commandHandler(char *command,int clientFd,int serverFd,List **countriesList,hashTable *filePathsHashTable, hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize){

    char *cleanCommand = strtok(command," ");

    int commandSize = strlen(command) + 1;
    write(serverFd, &commandSize, sizeof(int));
    write(serverFd, command, commandSize);

    //Call the function implementing the command given
    if(strcmp(cleanCommand,"fileReading") == 0){
        fileReading(clientFd,serverFd,countriesList,filePathsHashTable,records,countryHashTable,diseaseHashTable,bucketSize);
    }else if(strcmp(cleanCommand,"diseaseFrequency") == 0){
        diseaseFrequency(serverFd,diseaseHashTable ,countryHashTable,bucketSize);
    }else if(strcmp(cleanCommand,"topk-AgeRanges") == 0){
        topkAgeRanges(serverFd,countryHashTable ,bucketSize);
    }else if(strcmp(cleanCommand,"searchPatientRecord") == 0){
        searchPatientRecord(serverFd, records, bucketSize);
    }else if(strcmp(cleanCommand,"numPatientAdmissions") == 0){
        numPatientAdmissions(serverFd,countryHashTable,bucketSize);
    }else if(strcmp(cleanCommand,"numPatientDischarges") == 0){
        numPatientDischarges(serverFd,countryHashTable,bucketSize);
    }else{
    //  printHelp();
    }

    endComm(serverFd);
}

/*******************************************************
 *                                                     *
 *                /fileReading Command                 *
 *                                                     *
 *******************************************************/

//Worker (server) reads data from files
void fileReading(int clientFd, int serverFd ,List **countriesList,hashTable *filePathsHashTable, hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize){
    
    int numOfEntries = bucketSize / sizeof(bucketEntry);  
    int responseSize;
    char *filePath = NULL;
    char *tmpCountry, *inCountry,*country, *tmpDate,*inDate,*date, *trash, *response;
    FILE *filePtr;

    bucket *currBucket;
    bucketEntry *currEntry;

    avlTree *valueTree;

    listNode *countryNode;
    List *dateEntriesList;

    struct Date currDate;

    initializeList(countriesList);

    //Read from masters named pipe while end of communication is not given
    sourceRead(clientFd,&filePath);
    while(strcmp(filePath,"!") != 0){

        
        if((filePath != NULL) && (strcmp(filePath,"!") != 0)){
        
            //Find the country whos data are being read
            tmpCountry = (char *) malloc((strlen(filePath) + 1) * sizeof(char));
            strcpy(tmpCountry,filePath);
            strtok(tmpCountry,"/");
            strtok(NULL,"/");
            inCountry = strtok(NULL, "/");

            if((country = (char *)malloc((strlen(inCountry) + 1) * sizeof(char))) == NULL){
                perror("Error while allocating space for country name\n");
                exit(7);
            }
            strcpy(country,inCountry);

            free(tmpCountry);

            initializeListNode(&countryNode,country);
            insertFirst((*countriesList),countryNode);

            //Get the files contained in the directory
            listDirEntries(filePath,&dateEntriesList);

            //For each file
            listNode *tmp = dateEntriesList->first;
            while(tmp != NULL){

                if((filePtr = fopen((char *)tmp->payload,"r")) == NULL){
                    perror("Error while opening file for reading");
                    continue;
                }

                //Find the date represented by the file
                tmpDate = (char *) malloc((strlen((char *)tmp->payload) + 1) * sizeof(char));
                strcpy(tmpDate,(char *)tmp->payload);
                strtok(tmpDate,".");
                strtok(tmpDate,"/");
                strtok(NULL,"/");
                strtok(NULL,"/");
                inDate = strtok(NULL,"/");
                   
                if((date = (char *)malloc((strlen(inDate) + 1) * sizeof(char))) == NULL){
                    perror("Error while allocating space for date\n");
                    exit(7);
                }
                strcpy(date,inDate);

                free(tmpDate);

                //Read patient records contained in the file
                readPatientRecords(filePtr,records,countryHashTable,diseaseHashTable,bucketSize,country,date);
                    
                fclose(filePtr);

                //Start writting statistics in named pipe
                responseSize = strlen(date) + 1;
                if(write(serverFd,&responseSize,sizeof(int)) == -1){
                    perror("Error using write syscall in file Reading\n");
                    exit(7);
                }
                if(write(serverFd,date,responseSize) == -1){
                    perror("Erro using write syscall in file Reading\n");
                    exit(7);
                }

                responseSize = strlen(country) + 1;
                if(write(serverFd,&responseSize,sizeof(int)) == -1){
                    perror("Error using write syscall in file Reading\n");
                    exit(7);
                }
                if(write(serverFd,country,responseSize) == -1){
                    perror("Erro using write syscall in file Reading\n");
                    exit(7);
                }

                parseDate(date,&currDate);

                //Write statistics for each disease
                int diseaseHashTableSize = diseaseHashTable->tableSize;
                for(int i = 0; i < diseaseHashTableSize; i++){    
                    currBucket = diseaseHashTable->table[i];
                    //If the bucket list exists
                    if(currBucket != NULL){
                        //For each bucket
                        while(currBucket != NULL){
                            //For each entry in the bucket
                            for(int j = 0; j < numOfEntries; j++){
                                currEntry = currBucket->entries[j];
                                //If entry exists
                                if(currEntry != NULL){
                                
                                    valueTree = (avlTree *)currEntry->value;

                                    responseSize = strlen(*currEntry->key) + 1;
                                    response = (char *)malloc(responseSize * sizeof(char));
                                    sprintf(response, "%s",(*currEntry->key));
                                    
                                    if(write(serverFd,&responseSize,sizeof(int)) == -1){
                                        perror("Error using write syscall in list countries\n");
                                        exit(7);
                                    }
                                    if(write(serverFd,response,responseSize) == -1){
                                        perror("Erro using write syscall in list countries\n");
                                        exit(7);
                                    }
                                    free(response);
                                    
                                    char *ageAndDiseaseAndCountry;

                                    if((ageAndDiseaseAndCountry = (char *) malloc((strlen((*currEntry->key)) + strlen("020") + strlen(country) + 4) * sizeof(char))) == NULL){
                                        perror("Error while allocating space for ageAndDisease string\n");
                                        exit(7);
                                    }
                                    sprintf(ageAndDiseaseAndCountry,"%s+%s+%d+%d",(*currEntry->key),country,0,20);

                                    int minusTwenty = dateRangeEntries(valueTree->head,&currDate,&currDate,"age+disease+country",ageAndDiseaseAndCountry);
                                    free(ageAndDiseaseAndCountry);

                                    responseSize = strlen("Age range 0-20 years:  cases") + sizeof(intmax_t) + 1;
                                    response = (char *)malloc(responseSize * sizeof(char));
                                    
                                    memset(response,0,responseSize);
                                    sprintf(response, "Age range 0-20 years: %d cases",minusTwenty);
                                    
                                    if(write(serverFd,&responseSize,sizeof(int)) == -1){
                                        perror("Error using write syscall in list countries\n");
                                        exit(7);
                                    }
                                    if(write(serverFd,response,responseSize) == -1){
                                        perror("Error using write syscall in list countries\n");
                                        exit(7);
                                    }
                                    free(response);

                                    if((ageAndDiseaseAndCountry = (char *) malloc((strlen((*currEntry->key)) + strlen("2140") + strlen(country) + 4) * sizeof(char))) == NULL){
                                        perror("Error while allocating space for ageAndDisease string\n");
                                        exit(7);
                                    }
                                    sprintf(ageAndDiseaseAndCountry,"%s+%s+%d+%d",(*currEntry->key),country,21,40);

                                    int minusFourty = dateRangeEntries(valueTree->head,&currDate,&currDate,"age+disease+country",ageAndDiseaseAndCountry);
                                    free(ageAndDiseaseAndCountry);

                                    responseSize = strlen("Age range 21-40 years:  cases") + sizeof(intmax_t) + 1;
                                    response = (char *)malloc(responseSize * sizeof(char));
                                    
                                    memset(response,0,responseSize);
                                    sprintf(response, "Age range 21-40 years: %d cases",minusFourty);
                                    
                                    if(write(serverFd,&responseSize,sizeof(int)) == -1){
                                        perror("Error using write syscall in list countries\n");
                                        exit(7);
                                    }
                                    if(write(serverFd,response,responseSize) == -1){
                                        perror("Error using write syscall in list countries\n");
                                        exit(7);
                                    }
                                    free(response);

                                    if((ageAndDiseaseAndCountry = (char *) malloc((strlen((*currEntry->key)) + strlen("4160") + strlen(country) + 4) * sizeof(char))) == NULL){
                                        perror("Error while allocating space for ageAndDisease string\n");
                                        exit(7);
                                    }
                                    sprintf(ageAndDiseaseAndCountry,"%s+%s+%d+%d",(*currEntry->key),country,41,60);

                                    int minusSixty = dateRangeEntries(valueTree->head,&currDate,&currDate,"age+disease+country",ageAndDiseaseAndCountry);
                                    free(ageAndDiseaseAndCountry);

                                    responseSize = strlen("Age range 41-60 years:  cases") + sizeof(intmax_t) + 1;
                                    response = (char *)malloc(responseSize * sizeof(char));
                                    
                                    memset(response,0,responseSize);
                                    sprintf(response, "Age range 41-60 years: %d cases",minusSixty);
                                    
                                    if(write(serverFd,&responseSize,sizeof(int)) == -1){
                                        perror("Error using write syscall in list countries\n");
                                        exit(7);
                                    }
                                    if(write(serverFd,response,responseSize) == -1){
                                        perror("Error using write syscall in list countries\n");
                                        exit(7);
                                    }
                                    free(response);

                                    if((ageAndDiseaseAndCountry = (char *) malloc((strlen((*currEntry->key)) + strlen("61120") + strlen(country) + 4) * sizeof(char))) == NULL){
                                        perror("Error while allocating space for ageAndDisease string\n");
                                        exit(7);
                                    }
                                    sprintf(ageAndDiseaseAndCountry,"%s+%s+%d+%d",(*currEntry->key),country,61,120);

                                    int plusSixty = dateRangeEntries(valueTree->head,&currDate,&currDate,"age+disease+country",ageAndDiseaseAndCountry);
                                    free(ageAndDiseaseAndCountry);

                                    responseSize = strlen("Age range 60+ years:  cases") + sizeof(intmax_t) + 1;
                                    response = (char *)malloc(responseSize * sizeof(char));
                                    
                                    memset(response,0,responseSize);
                                    sprintf(response, "Age range 60+ years: %d cases",plusSixty);
                                    
                                    if(write(serverFd,&responseSize,sizeof(int)) == -1){
                                        perror("Error using write syscall in list countries\n");
                                        exit(7);
                                    }
                                    if(write(serverFd,response,responseSize) == -1){
                                        perror("Error using write syscall in list countries\n");
                                        exit(7);
                                    }
                                    free(response);

                                }
                            }   
                            currBucket = currBucket->next;
                        }
                    }
                }

                free(date);
                tmp = tmp->next;
            }

            char *tmpFilePath;
            if((tmpFilePath = (char *)malloc((strlen(filePath) + 1) * sizeof(char))) == NULL){
                perror("Error while allocating space for tmpFilePath\n");
                exit(7);
            }
            strcpy(tmpFilePath,filePath);

            //In a hash table keep the pairs (key: filePath, value: listOfFiles)
            insertElementInHash(filePathsHashTable,tmpFilePath,dateEntriesList,bucketSize);
        }

        free(filePath);
        sourceRead(clientFd,&filePath);
    }
    free(filePath);
}

/*******************************************************
 *                                                     *
 *              /diseaseFrequency Command              *
 *                                                     *
 *******************************************************/



void diseaseFrequency(int serverFd, hashTable *diseaseHashTable, hashTable *countryHashTable, int bucketSize){
    
    int numOfEntries = bucketSize / sizeof(bucketEntry);  
    char *option,*diseaseId, *date1, *date2, *country;

    date Date1, Date2;

    bucket *currBucket, *tmpBucket;
    bucketEntry *currEntry, *tmpEntry;
    avlTree *valueTree;

    //Gathere all given arguments
    if((option = strtok(NULL, " ")) == NULL){
//       printf("\nCorrect use of command is:\n");
//        printf("\tdiseaseFrequency <diseaseId> <date1> <date2> [country]\n");
        fprintf(stderr,"ERROR\n");
        
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
        fprintf(stderr,"ERROR\n");
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
        fprintf(stderr,"ERROR\n");
        free(diseaseId);
        free(date1);
        
        return;
    }
    if((date2 = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for date2\n");
        exit(7);
    }
    strcpy(date2,option);

    char *response;
    int responseSize;

    responseSize = sizeof(intmax_t) + 1;
    response = (char *) malloc(responseSize * sizeof(char));

    memset(response,0,responseSize);

    //If no country name was given
    if((option = strtok(NULL, " ")) == NULL){      
        
        //Both arguments have been given
        parseDate(date1,&Date1);
        parseDate(date2,&Date2);

        //Check that date1 is previous than date2
        if(compareDates(&Date1,&Date2) > 0){
//            printf("\nExit Date can not be previous than Entry Date...\n");
            fprintf(stderr,"ERROR\n");
            free(diseaseId);
            free(date1);
            free(date2);

            return;
        }

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
                            //Write in named pipe number of patients of the disease in the given date range
                            
                            sprintf(response,"%d",dateRangeEntries(valueTree->head,&Date1,&Date2,"default",NULL));

                            if(write(serverFd, &responseSize, sizeof(int)) == -1){
                                perror("Error while writing in named pipe\n");
                                exit(7);
                            }
                            if(write(serverFd, response, responseSize) == -1){
                                perror("Error while writing in named pipe\n");
                                exit(7);
                            }

                            free(response);
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
        sprintf(response,"0");
        
        if(write(serverFd, &responseSize, sizeof(int)) == -1){
            perror("Error while writing in named pipe\n");
            exit(7);
        }
        if(write(serverFd, response, responseSize) == -1){
            perror("Error while writing in named pipe\n");
            exit(7);
        }

        free(response);
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
        fprintf(stderr,"ERROR\n");
        
        free(response);
        free(diseaseId);
        free(country);
        free(date1);
        free(date2);

        return;
    }
    
    //Since the desired number of patients can be found both by diseaseHashTable and countryHashTabl
    currBucket = diseaseHashTable->table[stringHash(diseaseHashTable,diseaseId)];
    tmpBucket = countryHashTable->table[stringHash(countryHashTable,country)];

    int foundEntry;
    int diseaseTreeEntries = 0;
    int countryTreeEntries = 0;

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

    if(foundEntry == 0){
        
        free(response);
        free(diseaseId);
        free(country);
        free(date1);
        free(date2);

        return;
    }


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

    //Compare number of nodes in each avlTree and chose the one with the smallest
    if(diseaseTreeEntries <= countryTreeEntries){
        if(currEntry != NULL){
            valueTree = (avlTree *)currEntry->value;
            sprintf(response,"%d",dateRangeEntries(valueTree->head,&Date1,&Date2,"country",country));
        
            write(serverFd, &responseSize, sizeof(int));
            write(serverFd, response, responseSize);
        }else{
            sprintf(response,"0");

            write(serverFd, &responseSize, sizeof(int));
            write(serverFd, response, responseSize);
        }
    }else{
        if(tmpEntry != NULL){
            valueTree = (avlTree *)tmpEntry->value;
            sprintf(response,"%d",dateRangeEntries(valueTree->head,&Date1,&Date2,"disease",diseaseId));
        
            write(serverFd, &responseSize, sizeof(int));
            write(serverFd, response, responseSize);
        }else{
            sprintf(response,"0");
        
            write(serverFd, &responseSize, sizeof(int));
            write(serverFd, response, responseSize);
        }
    }
    free(response);
    free(diseaseId);
    free(country);
    free(date1);
    free(date2);
}



/*******************************************************
 *                                                     *
 *              /top-kAgeRanges Command                 *
 *                                                     *
 *******************************************************/

void topkAgeRanges(int serverFd,hashTable *countryHashTable ,int bucketSize){

    int k,totalDiseaseCount;
    int numOfEntries = bucketSize / sizeof(bucketEntry);  
    char *option,*country, *disease,*date1, *date2;
    date Date1, Date2;

    bucket *countryBucket, *currBucket;
    bucketEntry *countryEntry, *currEntry;
    avlTree *valueTree;
    hashTable *kAgeRangesHashTable;


    //Gathere all given arguments
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\t/topk-AgeRanges k <country> <disease> <date1> <date2>\n");
        fprintf(stderr,"ERROR\n");
        
        return;
    }
    k = atoi(option);
    if(k > 4){
        k = 4;
    }

    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\t/topk-AgeRanges k <country> <disease> <date1> <date2>\n");
        fprintf(stderr,"ERROR\n");

        return;
    }
    if((country = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for diseaseId\n");
        exit(7);
    }
    strcpy(country,option);

    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\t/topk-AgeRanges k <country> <disease> <date1> <date2>\n");
        fprintf(stderr,"ERROR\n");
        free(country);

        return;
    }
    if((disease = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for diseaseId\n");
        exit(7);
    }
    strcpy(disease,option);

    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\tDate1 is mandatory\n");
        fprintf(stderr,"ERROR\n");
        free(country);
        free(disease);

        return;
    }
    if((date1 = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for date1\n");
        exit(7);
    }
    strcpy(date1,option);

    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nSince date1 has been given a date2 is mandatory.\n");
        fprintf(stderr,"ERROR\n");
        free(country);
        free(disease);
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
        fprintf(stderr,"ERROR\n");
        free(country);
        free(disease);
        free(date1);
        free(date2);

        return;
    }

    int minusTwenty;
    int minusFourty;
    int minusSixty;
    int plusSixty;

    int foundCountry = 0;

    totalDiseaseCount = 0;

    initializeHashTable(&kAgeRangesHashTable,4);

    //Find bucket list containing given country
    countryBucket = countryHashTable->table[stringHash(countryHashTable,country)];

    int countryHashTableSize = countryHashTable->tableSize;
    //If the bucket list exists
    if(countryBucket != NULL){
        //For each bucket
        while(countryBucket != NULL){
            //For each entry in the bucket
            for(int j = 0; j < numOfEntries; j++){
                countryEntry = countryBucket->entries[j];
                //If entry exists
                if(countryEntry != NULL){
                    valueTree = (avlTree *)countryEntry->value;
                
                    //If entry key is the desired country
                    if(strcmp((*countryEntry->key),country) == 0){

                        char *ageAndDisease;

                        if((ageAndDisease = (char *) malloc((strlen(disease) + strlen("020") + 3) * sizeof(char))) == NULL){
                            perror("Error while allocating space for ageAndDisease string\n");
                            exit(12);
                        }
                        sprintf(ageAndDisease,"%s+%d+%d",disease,0,20);

                        minusTwenty = dateRangeEntries(valueTree->head,&Date1,&Date2,"age+disease",ageAndDisease);
                        free(ageAndDisease);

                        if((ageAndDisease = (char *) malloc((strlen(disease) + strlen("2140") + 3) * sizeof(char))) == NULL){
                            perror("Error while allocating space for ageAndDisease string\n");
                            exit(12);
                        }
                        sprintf(ageAndDisease,"%s+%d+%d",disease,21,40);
                        minusFourty = dateRangeEntries(valueTree->head,&Date1,&Date2,"age+disease",ageAndDisease);
                        free(ageAndDisease);

                        if((ageAndDisease = (char *) malloc((strlen(disease) + strlen("4160") + 3) * sizeof(char))) == NULL){
                            perror("Error while allocating space for ageAndDisease string\n");
                            exit(12);
                        }
                        sprintf(ageAndDisease,"%s+%d+%d",disease,41,60);
                        minusSixty = dateRangeEntries(valueTree->head,&Date1,&Date2,"age+disease",ageAndDisease);
                        free(ageAndDisease);

                        if((ageAndDisease = (char *) malloc((strlen(disease) + strlen("61120") + 3) * sizeof(char))) == NULL){
                            perror("Error while allocating space for ageAndDisease string\n");
                            exit(12);
                        }
                        sprintf(ageAndDisease,"%s+%d+%d",disease,61,120);
                        plusSixty = dateRangeEntries(valueTree->head,&Date1,&Date2,"age+disease",ageAndDisease);
                        free(ageAndDisease);
                        totalDiseaseCount = dateRangeEntries(valueTree->head,&Date1,&Date2,"disease",disease);


                        insertElementInHash(kAgeRangesHashTable,"0-20",&minusTwenty,bucketSize);
                        insertElementInHash(kAgeRangesHashTable,"21-40",&minusFourty,bucketSize);
                        insertElementInHash(kAgeRangesHashTable,"41-60",&minusSixty,bucketSize);
                        insertElementInHash(kAgeRangesHashTable,"60+",&plusSixty,bucketSize);                  
                    
                        foundCountry = 1;
                        break;
                    }
                }
            }
            if(foundCountry){
                break;
            }   
            countryBucket = countryBucket->next;
        }
    }

    char *response;
    int responseSize;

    if(totalDiseaseCount > 0){
        if(foundCountry){

            bucketEntry *maxEntry = NULL;

            int kAgeRangesHashTableSize = kAgeRangesHashTable->tableSize;
            int timesToK;
            for(timesToK = 0; timesToK < k; timesToK++){
                for(int i = 0; i < kAgeRangesHashTableSize; i++){
                    //If the bucket list exists
                    if(kAgeRangesHashTable->table[i] != NULL){
                        currBucket = kAgeRangesHashTable->table[i];
                        //For each bucket
                        while(currBucket != NULL){
                            //For each entry in the bucket
                            for(int j = 0; j < numOfEntries; j++){
                                currEntry = currBucket->entries[j];
                                //If entry exists
                                if(currEntry != NULL){

                                    if(maxEntry == NULL){
                                        maxEntry = currEntry;
                                    }else{
                                        if((*(int *)maxEntry->value) < (*(int *)currEntry->value)){
                                            maxEntry = currEntry;
                                        }
                                    }
                                }
                            }
                            currBucket = currBucket->next;
                        }
                    }
                }               
                if(maxEntry != NULL){
                    if((*(int *)maxEntry->value) > 0){
                        
                        responseSize = strlen((*maxEntry->key)) + sizeof(intmax_t) + 5;
                        response = (char *) malloc(responseSize * sizeof(char));

                        memset(response,0,responseSize);

                        sprintf(response,"%s: %d%%",(*maxEntry->key),((*(int *)maxEntry->value) * 100)/totalDiseaseCount);

                        write(serverFd,&responseSize,sizeof(int));
                        write(serverFd,response,responseSize);

                        free(response);
                    }
                    (*(int *)maxEntry->value) = 0;
                    maxEntry = NULL;
                }
            }
        }
    }else{
        free(country);
        free(disease);
        free(date1);
        free(date2);
        freeHashTable(kAgeRangesHashTable,bucketSize,"primitive");

        return;
    }

    free(country);
    free(disease);
    free(date1);
    free(date2);
    freeHashTable(kAgeRangesHashTable,bucketSize,"primitive");
}


/*******************************************************
 *                                                     *
 *            /searchPatientRecord Command             *
 *                                                     *
 *******************************************************/



void searchPatientRecord(int serverFd, hashTable *records, int bucketSize){

    char *option, *recordId;
    
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\t/searchPatientRecord <recordId>\n");
//        printf("\nCorrect use of command is:\n");
        fprintf(stderr,"ERROR\n");

        return;
    }
    if((recordId = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for recordId\n");
        exit(7);
    }
    strcpy(recordId,option);

    bucket *currBucket;
    bucketEntry *currEntry;
    patientRecord *existingRecord;

    char *response;
    int responseSize;
    int numOfEntries = bucketSize / sizeof(bucketEntry);  

    int hashTableSize = records->tableSize;
    //Find the bucket list containing the given recordId
    currBucket = records->table[stringHash(records,recordId)];
    //If the bucket list exists
    if(currBucket != NULL){
        //For each bucket
        while(currBucket != NULL){
            //For each entry in the bucket
            for(int j = 0; j < numOfEntries; j++){
                currEntry = currBucket->entries[j];
                //If entry exists
                if(currEntry != NULL){
                    if(strcmp((*(currEntry->key)),recordId) == 0){
                        existingRecord = (patientRecord *)currEntry->value;
                        //Print the patient record found
                        responseSize = strlen(getRecordId(existingRecord)) + strlen(getFirstName(existingRecord)) + 
                                       strlen(getLastName(existingRecord)) + strlen(getDiseaseID(existingRecord)) +
                                       strlen(getEntryDate(existingRecord)) + strlen(getExitDate(existingRecord)) +  sizeof(intmax_t) + 6;
                        response = (char *) malloc(responseSize * sizeof(char));

                        memset(response,0,responseSize);

                        sprintf(response,"%s %s %s %s %d %s %s",getRecordId(existingRecord),getFirstName(existingRecord),getLastName(existingRecord),getDiseaseID(existingRecord),getAge(existingRecord),getEntryDate(existingRecord),getExitDate(existingRecord));

                        if(write(serverFd,&responseSize,sizeof(int)) == -1){
                            perror("Error while writing in named pipe\n");
                            exit(7);
                        }
                        if(write(serverFd,response,responseSize) == -1){
                            perror("Error while writing in named pip\n");
                            exit(7);
                        }

                        free(response);
                        free(recordId);

                        return;
                    }
                }
            }
            currBucket = currBucket->next;
        }
    }
    free(recordId);
}



/*******************************************************
 *                                                     *
 *           /numPatientsAdmissions Command            *
 *                                                     *
 *******************************************************/


void numPatientAdmissions(int serverFd,hashTable *countryHashTable,int bucketSize){
    
    int currentPatientsNum;
    char *option,*diseaseId, *date1, *date2;
    char *country = NULL;
    bucketEntry *currEntry;
    bucket *currBucket;
    avlTree *valueTree;
    date Date1, Date2; 

    int numOfEntries = bucketSize / sizeof(bucketEntry);
    
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\t/numPatientAdmissions <disease> <date1> <date2> [country]\n");
        fprintf(stderr,"ERROR\n");

        return;
    }
    if((diseaseId = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for recordId\n");
        exit(7);
    }
    strcpy(diseaseId,option);   

    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nDate 1 argument is mandatory\n");
        fprintf(stderr,"ERROR\n");
        free(diseaseId);

        return;
    }
    if((date1 = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for date1\n");
        exit(7);
    }
    strcpy(date1,option);
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nSince date1 has been given a date2 is mandatory.\n");
        fprintf(stderr,"ERROR\n");
        free(diseaseId);
        free(date1);

        return;
    }
    if((date2 = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for date2\n");
        exit(7);
    }
    strcpy(date2,option);

    char *response;
    int responseSize;

    int countryFound = 0;

    if((option = strtok(NULL, " ")) == NULL){

        parseDate(date1,&Date1);
        parseDate(date2,&Date2);

        //Check that date1 is previous than date2
        if(compareDates(&Date1,&Date2) > 0){
//            printf("\nExit Date can not be previous than Entry Date...\n");
            fprintf(stderr,"ERROR\n");
            free(diseaseId);
            free(date1);
            free(date2);

            return;
        }

        int countryHashTableSize = countryHashTable->tableSize;
        //Search each list of buckets of the countryHashTable
        for(int i = 0; i < countryHashTableSize; i++){
            //If the bucket list exists
            if(countryHashTable->table[i] != NULL){
                currBucket = countryHashTable->table[i];
                //For each bucket
                while(currBucket != NULL){
                    //For each entry in the bucket
                    for(int j = 0; j < numOfEntries; j++){
                        currEntry = currBucket->entries[j];
                        //If entry exists
                        if(currEntry != NULL){

                            responseSize = strlen((*currEntry->key)) +  sizeof(intmax_t) + 2;
                            response = (char *) malloc(responseSize * sizeof(char));

                            memset(response,0,responseSize);

                            valueTree = (avlTree *)currEntry->value;
                            sprintf(response,"%s %d",(*currEntry->key),dateRangeEntries(valueTree->head,&Date1,&Date2,"disease",diseaseId));
                        
                            if(write(serverFd,&responseSize,sizeof(int)) == -1){
                                perror("Error while writing in named pipe\n");
                                exit(7);
                            }
                            if(write(serverFd,response,responseSize) == -1){
                                perror("Error while writing in named pipe\n");
                                exit(7);
                            }

                            free(response);
                        }
                    }
                    currBucket = currBucket->next;
                }
            }
        }
    }else{

        parseDate(date1,&Date1);
        parseDate(date2,&Date2);

        //Check that date1 is previous than date2
        if(compareDates(&Date1,&Date2) > 0){
//            printf("\nExit Date can not be previous than Entry Date...\n");
            fprintf(stderr,"ERROR\n");
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

        countryFound = 0;

        //Find the bucket list containing the given country
        currBucket = countryHashTable->table[stringHash(countryHashTable,country)];
        //If the bucket list exists
        if(currBucket != NULL){
            //For each bucket
            while(currBucket != NULL){
                //For each entry in the bucket
                for(int j = 0; j < numOfEntries; j++){
                    currEntry = currBucket->entries[j];
                    //If entry exists
                    if(currEntry != NULL){
                        //If this is the country we are searching for
                        if(strcmp((*currEntry->key),country) == 0){
                            valueTree = (avlTree *)currEntry->value;

                            responseSize = strlen((*currEntry->key)) +  sizeof(intmax_t) + 2;
                            response = (char *) malloc(responseSize * sizeof(char));

                            memset(response,0,responseSize);

                            sprintf(response,"%s %d",(*currEntry->key),dateRangeEntries(valueTree->head,&Date1,&Date2,"disease",diseaseId));
                        
                            if(write(serverFd,&responseSize,sizeof(int)) == -1){
                                perror("Error while writing in named pip\n");
                                exit(7);
                            }
                            if(write(serverFd,response,responseSize) == -1){
                                perror("Error while writing in named pipe\n");
                                exit(7);
                            }

                            free(response);
                            countryFound = 1;
                            break;
                        }
                    }
                }
                if(countryFound){
                    break;
                }
                currBucket = currBucket->next;
            }
        }
    }

    free(diseaseId);
    free(date1);
    free(date2);
    free(country);
}


/*******************************************************
 *                                                     *
 *           /numPatientDischarges Command             *
 *                                                     *
 *******************************************************/


void numPatientDischarges(int serverFd, hashTable *countryHashTable,int bucketSize){
    
    int currentPatientsNum;
    char *option,*diseaseId, *date1, *date2;
    char *country = NULL;
    bucketEntry *currEntry;
    bucket *currBucket;
    avlTree *valueTree;
    date Date1, Date2; 

    int numOfEntries = bucketSize / sizeof(bucketEntry);
    
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nCorrect use of command is:\n");
//        printf("\t/numPatientDischarges <disease> <date1> <date2> [country]\n");
        fprintf(stderr,"ERROR\n");
        return;
    }
    if((diseaseId = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for recordId\n");
        exit(7);
    }
    strcpy(diseaseId,option);   

    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nDate 1 argument is mandatory\n");
        fprintf(stderr,"ERROR\n");
        free(diseaseId);
        return;
    }
    if((date1 = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for date1\n");
        exit(7);
    }
    strcpy(date1,option);
    if((option = strtok(NULL, " ")) == NULL){
//        printf("\nSince date1 has been given a date2 is mandatory.\n");
        fprintf(stderr,"ERROR\n");
        free(diseaseId);
        free(date1);
        return;
    }
    if((date2 = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
        perror("\nError while allocating space for date2\n");
        exit(7);
    }
    strcpy(date2,option);

    char *response;
    int responseSize;

    if((option = strtok(NULL, " ")) == NULL){

        parseDate(date1,&Date1);
        parseDate(date2,&Date2);

        //Check that date1 is previous than date2
        if(compareDates(&Date1,&Date2) > 0){
//            printf("\nExit Date can not be previous than Entry Date...\n");
            fprintf(stderr,"ERROR\n");
            free(diseaseId);
            free(date1);
            free(date2);
            return;
        }

        int countryHashTableSize = countryHashTable->tableSize;
        //Search each list of buckets of the countryHashTable
        for(int i = 0; i < countryHashTableSize; i++){
            //If the bucket list exists
            if(countryHashTable->table[i] != NULL){
                currBucket = countryHashTable->table[i];
                //For each bucket
                while(currBucket != NULL){
                    //For each entry in the bucket
                    for(int j = 0; j < numOfEntries; j++){
                        currEntry = currBucket->entries[j];
                        //If entry exists
                        if(currEntry != NULL){
                            valueTree = (avlTree *)currEntry->value;

                            responseSize = strlen((*currEntry->key)) +  sizeof(intmax_t) + 2;
                            response = (char *) malloc(responseSize * sizeof(char));

                            memset(response,0,responseSize);

                            sprintf(response,"%s %d",(*currEntry->key),dateRangeEntries(valueTree->head,&Date1,&Date2,"exited+disease",diseaseId));
                        
                            if(write(serverFd,&responseSize,sizeof(int)) == -1){
                                perror("Error while writing in named pipe\n");
                                exit(7);
                            }
                            if(write(serverFd,response,responseSize) == -1){
                                perror("Error while writing in named pipe\n");
                                exit(7);
                            }
                        
                            free(response);
                        }
                    }
                    currBucket = currBucket->next;
                }
            }
        }        
    }else{

        parseDate(date1,&Date1);
        parseDate(date2,&Date2);

        //Check that date1 is previous than date2
        if(compareDates(&Date1,&Date2) > 0){
//            printf("\nExit Date can not be previous than Entry Date...\n");
            fprintf(stderr,"ERROR\n");
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

        int countryFound = 0;

        //Find the bucket list containing the given country
        currBucket = countryHashTable->table[stringHash(countryHashTable,country)];
        //If the bucket list exists
        if(currBucket != NULL){
            //For each bucket
            while(currBucket != NULL){
                //For each entry in the bucket
                for(int j = 0; j < numOfEntries; j++){
                    currEntry = currBucket->entries[j];
                    //If entry exists
                    if(currEntry != NULL){
                        //If this is the country we are searching for
                        if(strcmp((*currEntry->key),country) == 0){
                            valueTree = (avlTree *)currEntry->value;

                            responseSize = strlen((*currEntry->key)) +  sizeof(intmax_t) + 2;
                            response = (char *) malloc(responseSize * sizeof(char));

                            memset(response,0,responseSize);

                            sprintf(response,"%s %d",(*currEntry->key),dateRangeEntries(valueTree->head,&Date1,&Date2,"exited+disease",diseaseId));
                        
                            if(write(serverFd,&responseSize,sizeof(int)) == -1){
                                perror("Error while writing in named pipe\n");
                                exit(7);
                            }
                            if(write(serverFd,response,responseSize) == -1){
                                perror("Error while writing in named pipe\n");
                                exit(7);
                            }
                        
                            free(response);

                            countryFound = 1;
                            break;
                        }
                    }
                }
                if(countryFound){
                    break;
                }
                currBucket = currBucket->next;
            }
        }
    }

    free(diseaseId);
    free(date1);
    free(date2);
    free(country);
}

/*******************************************************
 *                                                     *
 *                   Read New Files                    *
 *                                                     *
 *******************************************************/

void readNewFiles(hashTable *filePathsHashTable, hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize){

    FILE *filePtr;
    int numOfEntries = bucketSize / sizeof(bucketEntry);

    bucket *currBucket;
    bucketEntry *currEntry;

    List *currDirEntries;

    for(int i = 0; i < filePathsHashTable->tableSize; i++){
        //If the bucket list exists
        if(filePathsHashTable->table[i] != NULL){
            currBucket = filePathsHashTable->table[i];
            //For each bucket
            while(currBucket != NULL){
                //For each entry in the bucket
                for(int j = 0; j < numOfEntries; j++){
                    currEntry = currBucket->entries[j];
                    //If entry exists
                    if(currEntry != NULL){

                        listDirEntries((*currEntry->key),&currDirEntries);

                        List *tmpList = (List *)currEntry->value;

                        listNode *tmp = currDirEntries->first;
                        while(tmp != NULL){

                            if(stringExistsInList(tmpList,(char *)tmp->payload) == NULL){

                                if((filePtr = fopen((char *)tmp->payload,"r")) == NULL){
                                    perror("Error while opening file for reading");
                                    tmp = tmp->next;
                                    continue;
                                }

                                char * tmpCountry = (char *) malloc((strlen((*currEntry->key)) + 1) * sizeof(char));
                                strcpy(tmpCountry,(*currEntry->key));
                                strtok(tmpCountry,"/");
                                strtok(NULL,"/");
                                char *inCountry = strtok(NULL, "/");

                                char *country;
                                if((country = (char *)malloc((strlen(inCountry) + 1) * sizeof(char))) == NULL){
                                    perror("Error while allocating space for country name\n");
                                    exit(7);
                                }
                                strcpy(country,inCountry);

                                free(tmpCountry);


                                char *tmpDate = (char *) malloc((strlen((char *)tmp->payload) + 1) * sizeof(char));
                                strcpy(tmpDate,(char *)tmp->payload);
                                strtok(tmpDate,".");
                                strtok(tmpDate,"/");
                                strtok(NULL,"/");
                                strtok(NULL,"/");
                                char *inDate = strtok(NULL,"/");
                                
                                char *date;
                                if((date = (char *)malloc((strlen(inDate) + 1) * sizeof(char))) == NULL){
                                    perror("Error while allocating space for date\n");
                                    exit(7);
                                }
                                strcpy(date,inDate);

                                free(tmpDate);

                                readPatientRecords(filePtr,records,countryHashTable,diseaseHashTable,bucketSize,country,date);
                                    
                                fclose(filePtr);
                            
                                listNode *newFileNode;
                                initializeListNode(&newFileNode,tmp->payload);
                                insertFirst(tmpList,newFileNode);
                            }

                            tmp = tmp->next;
                        }
                    }
                }
                currBucket = currBucket->next;
            }
        }
    }
}