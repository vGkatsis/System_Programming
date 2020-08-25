#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h> 
#include <fcntl.h>
#include <errno.h>
#include <poll.h>
#include <math.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../hdrs/hashTable.h"
#include "../hdrs/linked_list.h"
#include "../hdrs/communication.h" 
#include "../hdrs/signalHandlers.h"

#define BUCKET_SIZE 64
#define COUNTRY_HASH_NUM 10
#define DISEASE_HASH_NUM 10
#define RECORDS_HASH_NUM 25
#define FILEPATHS_HASH_NUM 25

int main(int argc, char **argv){

    int clientFd;
    int serverFd;
    int whoServerPort;
    int workerSocket, workerConnSocket;
    int whoServerSocket;
    int numOfWorkers;
    char clientNamedPipePath[42] = "./named_pipes/client_named_pipe";
    char serverNamedPipePath[42] = "./named_pipes/server_named_pipe";
    char *clientNamedPipe, *serverNamedPipe, *whoServerIp, *whoServerPortString;

    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;

    struct sockaddr_in whoServerAddress;

    List *countriesList;

    hashTable *records,*diseaseHashTable,*countryHashTable,*filePathsHashTable;

    signal(SIGINT, interuptWorker);
    signal(SIGQUIT, interuptWorker);

    //Initialize disease country and records hash tables
    initializeHashTable(&countryHashTable,COUNTRY_HASH_NUM);
    initializeHashTable(&diseaseHashTable,DISEASE_HASH_NUM);
    initializeHashTable(&filePathsHashTable,FILEPATHS_HASH_NUM);
    initializeHashTable(&records,RECORDS_HASH_NUM);


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

    whoServerPortString = NULL;
    whoServerIp = NULL;

    while(whoServerPortString == NULL){                                 //Read the port whoServer listens to
        poll(fdarray, 1, -1);
        if(fdarray[0].revents & POLLIN){
            sourceRead(clientFd,&whoServerPortString);
        }
    }
    while(whoServerIp == NULL){                                         //Read the the IP address of whoServer
        poll(fdarray, 1, -1);
        if(fdarray[0].revents & POLLIN){
            sourceRead(clientFd,&whoServerIp);
        }
    }
    whoServerPort = atoi(whoServerPortString);             //Get the whoServer port num as an integer
    free(whoServerPortString);

    endComm(serverFd);

    if((workerSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0){                         //Creating socket file descriptor
        perror("Error while creating workerSocket\n"); 
        exit(16); 
    } 
       
    if(setsockopt(workerSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){         //Setting socket options
        perror("Error while seting workerSocket options\n"); 
        exit(16);
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = 0;

    if(bind(workerSocket, (struct sockaddr *)&address, addrlen)<0){                    //Forcefully attaching socket to queryPortNum
        perror("WorkerSocket bind failed\n"); 
        exit(16);
    } 
    if(listen(workerSocket, 10) < 0){
        perror("listen"); 
        exit(16); 
    }

    if((getsockname(workerSocket,(struct sockaddr *)&address, &addrlen)) < 0){          //Get the address information of workerSocket
        perror("Error while getin worker port\n");
        exit(16);
    }

    if ((whoServerSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){                        //Create a socket to connect to whoServer 
        printf("Error while creating whoServer socket\n"); 
        exit(16); 
    }

    whoServerAddress.sin_family = AF_INET; 
    whoServerAddress.sin_port = htons(whoServerPort);

    if(inet_pton(AF_INET, whoServerIp, &whoServerAddress.sin_addr)<=0){                  //Convert whoServer IP address to binary network format 
        printf("Invalid whoServer IP address\n"); 
        exit(16); 
    }  

    if(connect(whoServerSocket, (struct sockaddr *)&whoServerAddress, sizeof(whoServerAddress)) < 0){
        printf("Failed to connect with who server socket\n"); 
        exit(16);
    }

    char workerPortString[12];                                              //Take workerPort as a string
    char numOfWorkersString[12];                                            //Take numberOfWorkers as a string
    sprintf(workerPortString,"%d",htons(address.sin_port));
    sprintf(numOfWorkersString,"%d",numOfWorkers);

    int responseSize = strlen(argv[2]) + 1;
    if(write(whoServerSocket,&responseSize,sizeof(int)) == -1){
        perror("Error using write syscall in worker\n");
        exit(7);
    }
    if(write(whoServerSocket,argv[2],responseSize) == -1){
        perror("Erro using write syscall in worker\n");
        exit(7);
    }

    responseSize = strlen(workerPortString) + 1;
    if(write(whoServerSocket,&responseSize,sizeof(int)) == -1){
        perror("Error using write syscall in worker\n");
        exit(7);
    }
    if(write(whoServerSocket,workerPortString,responseSize) == -1){
        perror("Erro using write syscall in worker\n");
        exit(7);
    }

    serverFd = whoServerSocket;

    int statsRead = 0;
    while(1){

        //If there are new data read them
        if(newWorkerData){
            readNewFiles(filePathsHashTable,records,countryHashTable,diseaseHashTable,BUCKET_SIZE);
            newWorkerData = 0;
        }

        //When client has given a command read it
        poll(fdarray, 1, -1);
        if(fdarray[0].revents & POLLIN){

            readCommand(clientFd,serverFd,&countriesList,filePathsHashTable,records,countryHashTable,diseaseHashTable,BUCKET_SIZE);
            if(statsRead == 0){
                statsRead++;
            }
        }

        //If stats have been read wait for incoming whoServer connections
        if(statsRead != 0){

            if((workerConnSocket = accept(workerSocket, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
                perror("accept"); 
                exit(7);
            }

            clientFd = workerConnSocket;
            serverFd = workerConnSocket;
        
            fdarray[0].fd = workerConnSocket;
        }

        if(workerInteruptionSignal){
            break;
        }
    }

    close(clientFd);
    close(serverFd); 

    free(whoServerIp);

    //Free data structures
    freeList(countriesList);

    freeHashTable(records,BUCKET_SIZE,"record");
    freeHashTable(filePathsHashTable,BUCKET_SIZE,"list");
    freeHashTable(countryHashTable,BUCKET_SIZE,"tree");
    freeHashTable(diseaseHashTable,BUCKET_SIZE,"tree");

    return 0;
}