#include "../hdrs/communication.h"

void namedPipeRead(int Fd, char **result){

    int readSize = -1;

    struct pollfd fdarray[1];
    fdarray[0].fd = Fd;
    fdarray[0].events = POLLIN;

    (*result) = NULL;

    while(readSize == -1){

        poll(fdarray, 1, -1);

        if(fdarray[0].revents & POLLIN){
            
            read(Fd,&readSize,sizeof(int));                          //Get the read size
        }
    }

    while((*result) == NULL){
        
        poll(fdarray, 1, -1);

        if(fdarray[0].revents & POLLIN){

            (*result) = (char *) malloc((readSize) * sizeof(char));    //Allocate enough space for the given reading
            read(Fd,(*result),readSize);
        }
    }

}

void clientWriteToServer(int numOfWorkers, char *clientNamedPipePath, char *stringToWrite){

    int clientFd[numOfWorkers];
    char *clientNamedPipe;

    for(int i = 0; i < numOfWorkers; i++){
        char processNumber[12];                                                 //Create name of current named pipe
        sprintf(processNumber,"%d",i);
        stringConcat(&clientNamedPipe,clientNamedPipePath,processNumber);
        
        int size = strlen(stringToWrite) + 1;

        clientFd[i] = open(clientNamedPipe, O_WRONLY | O_NONBLOCK);         //Open the current named pipe
        write(clientFd[i],&size,sizeof(int));                                 //Write in the named pipe the given string
        write(clientFd[i], stringToWrite,size);
        close(clientFd[i]);                                                 //Close the current named pipe
    
        free(clientNamedPipe);
    }
}

void serverDistributeCountries(List *countryDirList, int numOfWorkers, char *clientNamedPipePath, int singleWorker){

    char *clientNamedPipe;
    int listLength;
    int clientFd[numOfWorkers];

    listNode *countryListNode = getFirst(countryDirList);
    
    int nodeCounter = 0;
    listLength = countryDirList->length;
    while(nodeCounter < listLength){

        for(int i = 0; i < numOfWorkers; i++){                                      //For each worker
            if(nodeCounter < listLength){
                
                char processNumber[12];                                                 //Create name of current named pipe
                sprintf(processNumber,"%d",i);
                stringConcat(&clientNamedPipe,clientNamedPipePath,processNumber);

                if(singleWorker == -1){                                                 //Write on each workers named pipe

                    int size = (int)strlen(getPayload(countryListNode)) + 1;
                    clientFd[i] = open(clientNamedPipe, O_WRONLY | O_NONBLOCK);         //Open the current named pipe
                    write(clientFd[i],&size,sizeof(int));
                    write(clientFd[i], (char *)getPayload(countryListNode),size);         //Write in the named pipe a country dir path
                    close(clientFd[i]);                                                     //Close the current named pipe

                }else{

                    if(singleWorker == i){                                              //Write on specific workers named pipe

                        int size = (int)strlen(getPayload(countryListNode)) + 1;
                        clientFd[i] = open(clientNamedPipe, O_WRONLY | O_NONBLOCK);         //Open the current named pipe
                        write(clientFd[i],&size,sizeof(int));
                        write(clientFd[i], (char *)getPayload(countryListNode),size);            //Write in the named pipe a country dir path
                        close(clientFd[i]);                                                    //Close the current named pipe
                    }
                }
            
                countryListNode = countryListNode->next;
                nodeCounter++;

                free(clientNamedPipe);
            }else{
                break;
            }
        }

    }
}

void clientReadFromServer(int numOfWorkers, struct pollfd *serverFdArray, int *serverFd,logStats *logStatistics){

    int readFromWorkers = 0;                                                    //Number of wokers who have responded to the given command
    char *serverResponse;
    int frequency = 0;
    int totalRequests = 0;
    int successRequests = 0;
    int failedRequests = 0;
    while(readFromWorkers < numOfWorkers){                                      //While there are workers who havent responded to diseaseAggregator

        poll(serverFdArray,numOfWorkers,-1);                                    //Search which worker has responded

        for(int i = 0; i < numOfWorkers; i++){                                  //Find the worker who has responded
                
            if(((serverFdArray[i].revents & POLLIN))){

                namedPipeRead(serverFd[i], &serverResponse);                    //Reade from worker
                commandResultsHandler(serverFd[i],serverResponse,&frequency,readFromWorkers,numOfWorkers - 1,logStatistics);
                
                free(serverResponse);

                readFromWorkers++;
            }
        }
    }
}

void readCommand(int clientFd, int serverFd,List **countriesList,hashTable *filePathsHashTable, hashTable *records, hashTable *countryHashTable, hashTable *diseaseHashTable, int bucketSize, logStats *logStatistics){

    char *command;

    //Read from a named pipe
    namedPipeRead(clientFd,&command);
    //Server gets command read from named pipe        
    serverReadCommand(command,clientFd,serverFd,countriesList,filePathsHashTable,records,countryHashTable,diseaseHashTable,bucketSize,logStatistics);

    memset(command, 0, strlen(command) + 1);
    free(command);
}

void endComm(int serverFd){

    int responseSize = 2;
    
    //Write in a named pipe an exclamation mark as a communication end
    write(serverFd,&responseSize,sizeof(int));
    write(serverFd,"!",responseSize);
}