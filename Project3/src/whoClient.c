#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../hdrs/error_handler.h"
#include "../hdrs/whoClientFunctions.h"

int main(int argc, char **argv){

    char *serverIP;
    int  numOfThreads, serverPort;
    
    FILE *queryFile;

    if(!clientArgCheck(argc,argv,&queryFile,&numOfThreads,&serverPort,&serverIP)){       //Check if program has been called correctly
        return 1;
    }

    int sock, numberOfLines;
    char *line;
    size_t size = 1;
    struct sockaddr_in serv_addr;

    //Count the number of lines of the query file
    numberOfLines = 1;
    while(!feof(queryFile))
    {
        char ch = fgetc(queryFile);
        if(ch == '\n'){
            numberOfLines++;
        }
    }
    rewind(queryFile);

    //If number of queries is less than number of threads then reduce number of threads
    if(numberOfLines < numOfThreads){
        numOfThreads = numberOfLines;
    }

    //Open the socket to communicate with the server
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("Error while creating client socket\n"); 
        return 2; 
    } 

    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(serverPort);

    //Transform the name of the IP address
    if(inet_pton(AF_INET, serverIP, &serv_addr.sin_addr)<=0)  
    { 
        printf("Invalid IP address\n"); 
        return 3; 
    }  

    //Try to connect with who server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("Failed to connect with socket\n"); 
        return 4; 
    } 

    if(((line = (char*) malloc(size * sizeof(char)))) == NULL){
        perror("\nError While Allocating Memmory For Reading From Query File\n");
        exit(1);
    }

    pthread_t threads[numOfThreads];
    socketCommand socketCommandPair[numOfThreads];
    
    int threadCount = 0;
    char buffer[1024] = {0}; 
    //For each line in the query file
    while(getline(&line,&size,queryFile) != -1){
        line = strtok(line,"\n");

        //Hold a pair of coket and command
        socketCommandPair[threadCount].socket = sock;

        if(((socketCommandPair[threadCount].command = (char *) malloc((strlen(line) + 1) * sizeof(char))) == NULL)){
            perror("Error while allocating space for query\n");
            exit(15);
        }
        strcpy(socketCommandPair[threadCount].command,line);

        //Create a client thread
        pthread_create(&threads[threadCount],NULL,&threadReadCommand,&socketCommandPair[threadCount]);
    
        //If a bunch of threads is ready start them
        if(threadCount == (numOfThreads - 1)){
            sleep(1);

            pthread_cond_broadcast(&readyCond);

            for(int i = 0; i < numOfThreads; i++){
                pthread_join(threads[i],NULL);
            
                free(socketCommandPair[i].command);
            }

            pthread_mutex_init(&startMutex,NULL);
            pthread_cond_init(&readyCond,NULL);

            threadCount = -1;
        }

        threadCount++;
    }

    //Start any remaining threads
    if(((threadCount - 1) < (numOfThreads - 1)) && (threadCount != 0)){
        sleep(1);
        
        pthread_cond_broadcast(&readyCond);
    
        for(int i = 0; i < threadCount; i++){
            pthread_join(threads[i],NULL);
        
            free(socketCommandPair[i].command);
        }
    }

    free(line);

    return 0;
}