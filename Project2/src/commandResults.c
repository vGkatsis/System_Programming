#include "../hdrs/commandResults.h"

//Select the command for which results should be gathered
void commandResultsHandler(int serverFd, char *command, int *frequency, int curWorker ,int numOFWorkers, logStats *logStatistics){

    //Call the function implementing the command given
    if(strcmp(command,"listCountries") == 0){
        listCountriesResults(serverFd);
    }else if(strcmp(command,"fileReading") == 0){
        fileReadingResults(serverFd);
    }else if(strcmp(command,"diseaseFrequency") == 0){
        diseaseFrequencyResults(serverFd,frequency,curWorker,numOFWorkers);
    }else if(strcmp(command,"topk-AgeRanges") == 0){
        topkAgeRangesResults(serverFd);
    }else if(strcmp(command,"searchPatientRecord") == 0){
        searchPatientRecordResults(serverFd);
    }else if(strcmp(command,"numPatientAdmissions") == 0){
        numPatientAdmissionsResults(serverFd);
    }else if(strcmp(command,"numPatientDischarges") == 0){
        numPatientDischargesResults(serverFd);
    }else if(strcmp(command,"logStatistics") == 0){
        logStatisticsResults(serverFd,curWorker,numOFWorkers,logStatistics);
    }else if(strcmp(command,"!") == 0){
        return;
    }else{
        wrongCommand(serverFd);
    }
}

//Gather results of fileReading function
void fileReadingResults(int serverFd){

    char *commandResult;
    
    int storagePosition = 0;
    char **resultStorage;

    //Read from servers named pipe
    namedPipeRead(serverFd, &commandResult);
    while(strcmp(commandResult,"!") != 0){

        //Save fileReading results in string array
        if(storagePosition == 0){

            if((resultStorage = (char **)malloc(1 * sizeof(char *))) == NULL){
                perror("Error while allocating space for result storage\n");
                exit(14);
            }
        }else{

            if((resultStorage = (char **) realloc(resultStorage,((storagePosition + 1) * sizeof(char *)))) == NULL){
                perror("Error while reallocating space for result storage\n");
                exit(14);
            }
        }
        resultStorage[storagePosition] = commandResult;
        storagePosition++;

        namedPipeRead(serverFd, &commandResult);
    }
    free(commandResult);

    //If there are results print them
    if(storagePosition > 0){

        for(int i = 0; i < storagePosition; i++){
            printf("%s\n",resultStorage[i]);
        }
    }

    //Free string array
    for(int i = 0; i < storagePosition; i++){
        free(resultStorage[i]);
    }
    free(resultStorage);
}

//Gather results of listCountries command
void listCountriesResults(int serverFd){

    char *commandResult;

    //Read from servers named pipe
    namedPipeRead(serverFd, &commandResult);
    while(strcmp(commandResult,"!") != 0){

        //Print listCOuntries results
        printf("%s\n",commandResult);
        free(commandResult);
        namedPipeRead(serverFd, &commandResult);
    }
    free(commandResult);
}

//Gather results of diseaseFrequency command
void diseaseFrequencyResults(int serverFd, int *frequency, int curWorker,int numOfWorkers){

    char *commandResult;

    //Read from servers named pipe
    namedPipeRead(serverFd, &commandResult);
    while(strcmp(commandResult,"!") != 0){

        //Gather and add results of diseaseFrequency command
        (*frequency) += atoi(commandResult);
        free(commandResult);
        namedPipeRead(serverFd, &commandResult);
    }
    free(commandResult);
    
    //When all results are gathered print them
    if(curWorker == numOfWorkers){
        printf("%d\n",(*frequency));
    }
}


//Gather results of topk-AgeRanges command
void topkAgeRangesResults(int serverFd){

    char *commandResult;

    //Read from servers named pipe
    namedPipeRead(serverFd, &commandResult);
    while(strcmp(commandResult,"!") != 0){

        //Print results of topkAgeRanges command
        printf("%s\n",commandResult);
        free(commandResult);
        namedPipeRead(serverFd, &commandResult);
    }
    free(commandResult);
}

//Gather results of searchPatientRecord command
void searchPatientRecordResults(int serverFd){

    char *commandResult;

    //Read from servers named pipe
    namedPipeRead(serverFd, &commandResult);
    while(strcmp(commandResult,"!") != 0){

        //Print results of searchPatientRecord command
        printf("%s\n",commandResult);
        free(commandResult);
        namedPipeRead(serverFd, &commandResult);
    }
    free(commandResult);
}

//Gather results of numPatientAdmissions command
void numPatientAdmissionsResults(int serverFd){

    char *commandResult;

    //Read from servers named pipe
    namedPipeRead(serverFd, &commandResult);
    while(strcmp(commandResult,"!") != 0){

        //Print results of numPatientAdmissions command
        printf("%s\n",commandResult);
        free(commandResult);
        namedPipeRead(serverFd, &commandResult);
    }
    free(commandResult);
}

//Gather results of numPatientDischarges command
void numPatientDischargesResults(int serverFd){

    char *commandResult;

    //Read from servers named pipe
    namedPipeRead(serverFd, &commandResult);
    while(strcmp(commandResult,"!") != 0){

        //Print numPatientDischarges command
        printf("%s\n",commandResult);
        free(commandResult);
        namedPipeRead(serverFd, &commandResult);
    }
    free(commandResult);
}

//Gathere intel when wrong command was given
void wrongCommand(int serverFd){

    char *commandResult;

    //Read from servers named pipe until end of communication is reached
    namedPipeRead(serverFd, &commandResult);
    while(strcmp(commandResult,"!") != 0){

        free(commandResult);
        namedPipeRead(serverFd, &commandResult);
    }
    free(commandResult);
}

//Gather log statistics from workers
void logStatisticsResults(int serverFd, int curWorker, int numOfWorkers, logStats *logStatistics){

    char *commandResult;
    int totalFLag = 0;
    int successFlag = 0;
    int failedFlag = 0; 

    //Read from servers named pipe
    namedPipeRead(serverFd, &commandResult);
    while(strcmp(commandResult,"!") != 0){

        //Check if the following result is total requests
        if(strcmp(commandResult,"TOTAL") == 0){
            totalFLag = 1;
            
            free(commandResult);
            namedPipeRead(serverFd, &commandResult);
            continue;
        }
        //Check if the following result is success requests
        if(strcmp(commandResult,"SUCCESS") == 0){
            successFlag = 1;
            totalFLag = 0;
            
            free(commandResult);
            namedPipeRead(serverFd, &commandResult);
            continue;
        }
        //Check if the following result is failed requests
        if(strcmp(commandResult,"FAILED") == 0){
            failedFlag = 1;
            totalFLag = 0;
            successFlag = 0;
            
            free(commandResult);
            namedPipeRead(serverFd, &commandResult);
            continue;
        }

        //Add results
        if(totalFLag == 1){
            logStatistics->totalRequests += atoi(commandResult);
        }else if(successFlag == 1){
            logStatistics->successRequests += atoi(commandResult);
        }else if(failedFlag == 1){
            logStatistics->failedRequests += atoi(commandResult);
        }else{  //If result read is a country add it to a list
            char * country;
            listNode *tmpNode;

            if((country = (char *)malloc((strlen(commandResult) + 1) * sizeof(char))) == 0){
                perror("Error while allocating space fro log country\n");
                exit(13);
            }
            strcpy(country,commandResult);
        
            initializeListNode(&tmpNode,country);
            insertFirst(logStatistics->countriesList,tmpNode);
        }

        free(commandResult);
        namedPipeRead(serverFd, &commandResult);
    }
    free(commandResult);

    //When all results are gathered create the log file
    if(curWorker == numOfWorkers){
        createLogFile(logStatistics);
    
        freeList(logStatistics->countriesList);
    }
}