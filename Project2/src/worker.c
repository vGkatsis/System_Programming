#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <math.h>
#include "../hdrs/hashTable.h"
#include "../hdrs/linked_list.h"
#include "../hdrs/communication.h" 
#include "../hdrs/signalHandlers.h"
#include "../hdrs/logFileStatistics.h"

#define BUCKET_SIZE 64
#define COUNTRY_HASH_NUM 10
#define DISEASE_HASH_NUM 10
#define RECORDS_HASH_NUM 25
#define FILEPATHS_HASH_NUM 25

int main(int argc, char **argv){

    int clientFd;
    int serverFd;
    char clientNamedPipePath[42] = "./named_pipes/client_named_pipe";
    char serverNamedPipePath[42] = "./named_pipes/server_named_pipe";
    char *clientNamedPipe, *serverNamedPipe;
    logStats logFileStatistics;
    List *countriesList;

    hashTable *records,*diseaseHashTable,*countryHashTable,*filePathsHashTable;

    signal(SIGINT, interuptWorker);
    signal(SIGQUIT, interuptWorker);

    //Initialize disease country and records hash tables
    initializeHashTable(&countryHashTable,COUNTRY_HASH_NUM);
    initializeHashTable(&diseaseHashTable,DISEASE_HASH_NUM);
    initializeHashTable(&filePathsHashTable,FILEPATHS_HASH_NUM);
    initializeHashTable(&records,RECORDS_HASH_NUM);

    initializeLogStats(&logFileStatistics);

    stringConcat(&clientNamedPipe,clientNamedPipePath,argv[1]);         //Create the names of named pipes
    stringConcat(&serverNamedPipe,serverNamedPipePath,argv[1]);


    clientFd = open(clientNamedPipe, O_RDONLY | O_NONBLOCK);            //Open the named pipes
    serverFd = open(serverNamedPipe, O_WRONLY | O_NONBLOCK);    

    free(clientNamedPipe);
    free(serverNamedPipe);

    struct pollfd fdarray[1];
    fdarray[0].fd = clientFd;
    fdarray[0].events = POLLIN;

    signal(SIGINT, interuptWorker);
    signal(SIGQUIT, interuptWorker);

    signal(SIGUSR1, newDataForWorker);

    endComm(serverFd);                                                  //Inform the client that the server is ready to receive commands

    while(1){

        //If there are new data read them
        if(newWorkerData){
            readNewFiles(filePathsHashTable,records,countryHashTable,diseaseHashTable,BUCKET_SIZE,&logFileStatistics);
            newWorkerData = 0;
        }

        //When client has given a command read it
        poll(fdarray, 1, -1);
        if(fdarray[0].revents & POLLIN){

            readCommand(clientFd,serverFd,&countriesList,filePathsHashTable,records,countryHashTable,diseaseHashTable,BUCKET_SIZE,&logFileStatistics);

        }

        if(workerInteruptionSignal){
            break;
        }
    }

    //Create the log file
    createLogFile(&logFileStatistics);

    close(clientFd);
    close(serverFd); 

    //Free data structures
    freeList(countriesList);

    freeHashTable(records,BUCKET_SIZE,"record");
    freeHashTable(filePathsHashTable,BUCKET_SIZE,"list");
    freeHashTable(countryHashTable,BUCKET_SIZE,"tree");
    freeHashTable(diseaseHashTable,BUCKET_SIZE,"tree");

    return 0;
}