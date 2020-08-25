#include "../hdrs/whoServerFunctions.h"

pthread_mutex_t startServerThread = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t readBufferMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t modifyWorkersNum = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t addWorkerPort = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t executeQuery = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t serverThreadReady = PTHREAD_COND_INITIALIZER;

extern int numOfWorkers;
extern int *workersPorts;

void *threadServerCommand(void *arg){

    circularBuffer *fdsBuffer = (circularBuffer *)arg;
    fdType currFdType;

    while(1){

        //Lock thread and wati for condition
        pthread_mutex_lock(&startServerThread);

        pthread_cond_wait(&serverThreadReady, &startServerThread);

        pthread_mutex_unlock(&startServerThread);

        //Lock all other threads from reading from buffer
        pthread_mutex_lock(&readBufferMutex);

        currFdType = removeFromCircular(fdsBuffer);

        //Unlock threads
        pthread_mutex_unlock(&readBufferMutex);

        struct pollfd fdarray[1];

        fdarray[0].fd = currFdType.fd;
        fdarray[0].events = POLLIN;

        int readResult, lineSize;
        char *command;

        readResult = 1;

        //If connection came from a worker
        if(currFdType.type == 'w'){

            int firstTwoRepetitions = 1;

            while((readResult > 0) && (firstTwoRepetitions < 3)){

                poll(fdarray, 1, -1);

                if(fdarray[0].revents & POLLIN){
                    read(currFdType.fd,&lineSize,sizeof(int));
                    printf("Size of command is %d\n",lineSize);
                }

                poll(fdarray, 1, -1);
                        
                if(fdarray[0].revents & POLLIN){
                    command = (char *) malloc(lineSize * sizeof(char));
                    readResult = read(currFdType.fd, command, lineSize);
                    if(readResult != 0){

                        //Read num of workers  
                        if(firstTwoRepetitions == 1){
                            pthread_mutex_lock(&modifyWorkersNum);
                            
                            if(numOfWorkers == 0){
                                numOfWorkers = atoi(command);
                            }
                            
                            pthread_mutex_unlock(&modifyWorkersNum);
                            
                            firstTwoRepetitions++;
                        //Read workerPorts table
                        }else if(firstTwoRepetitions == 2){
                            pthread_mutex_lock(&addWorkerPort);
                            
                            if(workersPorts == NULL){
                                if((workersPorts = (int *) malloc(numOfWorkers * sizeof(int))) == NULL){
                                    fprintf(stderr,"Error while allocating space for workers ports\n");
                                    exit(18);
                                }
                                
                                for(int i = 0; i < numOfWorkers; i++){
                                    workersPorts[i] = -1;
                                }

                                workersPorts[0] = atoi(command);
                            }else{
                                int i = 0;
                                while(workersPorts[i] != -1){
                                    i++;
                                }
                                workersPorts[i] = atoi(command);
                            }
                            
                            pthread_mutex_unlock(&addWorkerPort);
                            
                            firstTwoRepetitions++;
                        }else{
                            fprintf(stderr,"Error while reading workersNum and workersPort\n");
                        }
                    }
                    free(command);
                }
            }
            int workerFd[1];

            workerFd[0] = currFdType.fd;
            clientReadFromServer(1,fdarray,workerFd);
        }

        //If connection comes from a client
        if(currFdType.type == 'c'){

            int readCommand = 0;

            while(!readCommand){

                    poll(fdarray, 1, -1);

                    if(fdarray[0].revents & POLLIN){
                        read(currFdType.fd,&lineSize,sizeof(int));
                    }

                    poll(fdarray, 1, -1);
                        
                    if(fdarray[0].revents & POLLIN){
                        command = (char *) malloc(lineSize * sizeof(char));
                        readResult = read(currFdType.fd, command, lineSize);
                        if(readResult != 0){
                            
                            int workerFds[numOfWorkers];
                            struct pollfd workerFdArray[numOfWorkers];

                            struct sockaddr_in worker_addr;
                            int worker_addr_len = sizeof(worker_addr);

                            //Write the given command to each worker
                            for(int i = 0; i < numOfWorkers; i++){

                                if((workerFds[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
                                { 
                                    printf("Error while creating worker socket\n"); 
                                    exit(18); 
                                } 

                                worker_addr.sin_family = AF_INET; 
                                worker_addr.sin_port = htons(workersPorts[i]);

                                getpeername(workerFds[i],(struct sockaddr *)&worker_addr,(socklen_t *)&worker_addr_len);


                                if(connect(workerFds[i], (struct sockaddr *)&worker_addr, sizeof(worker_addr)) < 0){ 
                                    printf("Failed to connect with worker socket\n"); 
                                    
                                    exit(18); 
                                }

                                write(workerFds[i],&lineSize,sizeof(int));
                                write(workerFds[i],command,lineSize);
                            }

                            for(int i = 0; i < numOfWorkers; i++){

                                workerFdArray[i].fd = workerFds[i];
                                workerFdArray[i].events = POLLIN;
                            }

                            //Read the results from each client
                            printf("%s\n",command);
                            clientReadFromServer(numOfWorkers,workerFdArray,workerFds);

                            readCommand = 1;
                        }
                        free(command);
                    }
                }
            }
        }
}
