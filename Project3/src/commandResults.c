#include "../hdrs/commandResults.h"

//Select the command for which results should be gathered
void commandResultsHandler(int serverFd, char *command, int *frequency, int curWorker ,int numOFWorkers){

    //Call the function implementing the command given
    if(strcmp(command,"fileReading") == 0){
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
    sourceRead(serverFd, &commandResult);
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

        sourceRead(serverFd, &commandResult);
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

//Gather results of diseaseFrequency command
void diseaseFrequencyResults(int serverFd, int *frequency, int curWorker,int numOfWorkers){

    char *commandResult;

    //Read from servers named pipe
    sourceRead(serverFd, &commandResult);
    while(strcmp(commandResult,"!") != 0){

        //Gather and add results of diseaseFrequency command
        (*frequency) += atoi(commandResult);
        free(commandResult);
        sourceRead(serverFd, &commandResult);
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
    sourceRead(serverFd, &commandResult);
    while(strcmp(commandResult,"!") != 0){

        //Print results of topkAgeRanges command
        printf("%s\n",commandResult);
        free(commandResult);
        sourceRead(serverFd, &commandResult);
    }
    free(commandResult);
}

//Gather results of searchPatientRecord command
void searchPatientRecordResults(int serverFd){

    char *commandResult;

    //Read from servers named pipe
    sourceRead(serverFd, &commandResult);
    while(strcmp(commandResult,"!") != 0){

        //Print results of searchPatientRecord command
        printf("%s\n",commandResult);
        free(commandResult);
        sourceRead(serverFd, &commandResult);
    }
    free(commandResult);
}

//Gather results of numPatientAdmissions command
void numPatientAdmissionsResults(int serverFd){

    char *commandResult;

    //Read from servers named pipe
    sourceRead(serverFd, &commandResult);
    while(strcmp(commandResult,"!") != 0){
        
        //Print results of numPatientAdmissions command
        printf("%s\n",commandResult);
        free(commandResult);
        sourceRead(serverFd, &commandResult);
    }
    free(commandResult);
}

//Gather results of numPatientDischarges command
void numPatientDischargesResults(int serverFd){

    char *commandResult;

    //Read from servers named pipe
    sourceRead(serverFd, &commandResult);
    while(strcmp(commandResult,"!") != 0){

        //Print numPatientDischarges command
        printf("%s\n",commandResult);
        free(commandResult);
        sourceRead(serverFd, &commandResult);
    }
    free(commandResult);
}

//Gathere intel when wrong command was given
void wrongCommand(int serverFd){

    char *commandResult;

    //Read from servers named pipe until end of communication is reached
    sourceRead(serverFd, &commandResult);
    while(strcmp(commandResult,"!") != 0){

        free(commandResult);
        sourceRead(serverFd, &commandResult);
    }
    free(commandResult);
}