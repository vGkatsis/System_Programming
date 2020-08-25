#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include "../hdrs/error_handler.h"
#include "../hdrs/communication.h"
#include "../hdrs/circularBuffer.h"
#include "../hdrs/whoServerFunctions.h"

int numOfWorkers = 0;
int *workersPorts = NULL;

int main(int argc, char **argv){

    int queryPortNum, statisticsPortNum, numOfThreads, bufferSize;

    if(!serverArgCheck(argc,argv,&queryPortNum,&statisticsPortNum,&numOfThreads,&bufferSize)){       //Check if program has been called correctly
        return 1;
    }

    circularBuffer fdsBuffer;
    initializeCircular(&fdsBuffer,bufferSize);

    struct sockaddr_in address,statsAddress;

    int serverSocket, statsServerSocket, whoClientSocket ,workerSocket; 
    int opt = 1;
    int addrlen = sizeof(address);
    int flags;
    int statsAddrlen = sizeof(statsAddress);
    int *workerFds;
    char *buffer;

    struct pollfd fdarray[1];

    //Create numOfThreads threads
    pthread_t threads[numOfThreads];
    for(int i = 0; i < numOfThreads; i++){
        pthread_create(&threads[i],NULL,&threadServerCommand,&fdsBuffer);
    }

    //Create a socket for connection wtih the client
    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("Socket failure\n"); 
        return 2; 
    } 

    if(setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){         //Setting socket options
        perror("setsockopt"); 
        return 3;
    }

    flags = fcntl(serverSocket,F_GETFL,0);

    if(flags == -1){
        perror("Error while getin serverSocket flags\n");
        return 7;
    }

    fcntl(serverSocket, F_SETFL, flags | O_NONBLOCK);

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(queryPortNum);

    if(bind(serverSocket, (struct sockaddr *)&address, addrlen)<0){                    //Forcefully attaching socket to queryPortNum
        perror("bind failed"); 
        return 4;
    } 
    if(listen(serverSocket, 10) < 0){
        perror("listen"); 
        return 5; 
    }

    //Create a socket for connecting with workers
    if((statsServerSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0){                         //Creating socket file descriptor
        perror("Socket failure\n"); 
        return 2; 
    } 

    if(setsockopt(statsServerSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){         //Setting socket options
        perror("setsockopt"); 
        return 3;
    }

    flags = fcntl(statsServerSocket,F_GETFL,0);

    if(flags == -1){
        perror("Error while getin statsServerSocket flags\n");
        return 7;
    }

    fcntl(statsServerSocket, F_SETFL, flags | O_NONBLOCK);

    statsAddress.sin_family = AF_INET; 
    statsAddress.sin_addr.s_addr = INADDR_ANY; 
    statsAddress.sin_port = htons(statisticsPortNum);

    if(bind(statsServerSocket, (struct sockaddr *)&statsAddress, statsAddrlen)<0){                    //Forcefully attaching socket to queryPortNum
        perror("bind failed"); 
        return 4;
    } 
    if(listen(statsServerSocket, 10) < 0){
        perror("listen"); 
        return 5; 
    }
    
    while(1){

        //If circular buffer is not empty start a thread
        if(!circularIsEmpty(&fdsBuffer)){
            
            pthread_cond_signal(&serverThreadReady);
        }

        fdarray[0].fd = statsServerSocket;
        fdarray[0].events = POLLIN;

        //Listen for connections from a worker
        poll(fdarray, 1, 100);

        if(fdarray[0].revents & POLLIN){
            errno = 0;
            if((workerSocket = accept(statsServerSocket, (struct sockaddr *)&statsAddress,(socklen_t*)&statsAddrlen))<0){
                perror("accept"); 
                return 6;
            }

            if(errno != EAGAIN && errno != EWOULDBLOCK){

                insertInCircular(&fdsBuffer,workerSocket,'w');
            }
        }

        fdarray[0].fd = serverSocket;
        fdarray[0].events = POLLIN;

        //Listen for connections from the client
        poll(fdarray, 1, 100);

        if(fdarray[0].revents & POLLIN){
            errno = 0;
            if((whoClientSocket = accept(serverSocket, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
                perror("accept"); 
                return 6;
            }

            if(errno != EAGAIN && errno != EWOULDBLOCK){

                insertInCircular(&fdsBuffer,whoClientSocket,'c');
            }
        }
    }

    return 0;
}