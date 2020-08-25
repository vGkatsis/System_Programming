#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <poll.h>
#include "../hdrs/commands.h"
#include "../hdrs/linked_list.h"
#include "../hdrs/utilityFunctions.h"
#include "../hdrs/error_handler.h"
#include "../hdrs/signalHandlers.h"
#include "../hdrs/commandResults.h"
#include "../hdrs/logFileStatistics.h"


int main(int argc, char **argv){

    int numOfWorkers,bufferSize;
    char *inputDir = NULL;

    if(!argCheck(argc,argv,&numOfWorkers,&bufferSize,&inputDir)){       //Check if program has been called correctly
        return 1;
    }

    List *countryDirList, *logCountriesList;

    listDirEntries(inputDir,&countryDirList);                           //Get a list with all country directories

    if(countryDirList->length < numOfWorkers){                          //If numOfWorkers given is more than countri dirs change numOfWorkers to number of country dirs
        numOfWorkers = countryDirList->length;
    }

    int clientFd[numOfWorkers];                                             //Keep file descriptors where client(diseaseAggregator) writes
    int serverFd[numOfWorkers];                                             //Keep file descriptors where server(worker) writes
    char clientNamedPipePath[42] = "./named_pipes/client_named_pipe";       //Path and Name of client named pipes
    char serverNamedPipePath[42] = "./named_pipes/server_named_pipe";       //Path and Name of server named pipes
    char *clientNamedPipe, *serverNamedPipe;

    struct pollfd serverFdArray[numOfWorkers];                                //Struct used in poll function
    pid_t fatherId, waitId[numOfWorkers];

    logStats logStatistics;

    initializeLogStats(&logStatistics);
    initializeList(&logCountriesList);

    setCountriesList(&logStatistics,logCountriesList);

    fatherId = getpid();                                                //Process id of diseaseAggregator
    for(int i = 0; i < numOfWorkers; i++){                              //Create The Requested Number Of Workers

        char processNumber[12];
        sprintf(processNumber,"%d",i);
        stringConcat(&clientNamedPipe,clientNamedPipePath,processNumber);
        stringConcat(&serverNamedPipe,serverNamedPipePath,processNumber);

        mkfifo(clientNamedPipe,0777);                               //Create required named pipes
        mkfifo(serverNamedPipe,0777);

        serverFd[i] = open(serverNamedPipe, O_RDONLY | O_NONBLOCK);     //Open server named pipes for reading

        serverFdArray[i].fd = serverFd[i];                                    //Initialize fdarray struct to be used in poll()
        serverFdArray[i].events = POLLIN;                                     //Interested in insertion actions

        char *args[] = {"./worker",processNumber,NULL};                 //Each worker is given an identifier for later communication
        if((waitId[i] = fork()) == 0){
            execvp(args[0],args);                                       //Child processes become workers
            break;
        }
    
        free(clientNamedPipe);
        free(serverNamedPipe);
    }

    signal(SIGCHLD,handleChildInteruption);

    clientReadFromServer(numOfWorkers,serverFdArray,serverFd,&logStatistics);        //Wait until each worker is ready to get commands

    clientWriteToServer(numOfWorkers,clientNamedPipePath,"fileReading");             //Give the workers the fileReading command

    serverDistributeCountries(countryDirList,numOfWorkers,clientNamedPipePath,(-1));    //Distribute the countries to the workers
    
    clientWriteToServer(numOfWorkers,clientNamedPipePath,"!");                       //Write end comm after distribution ends

    clientReadFromServer(numOfWorkers,serverFdArray,serverFd,&logStatistics);        //Read response of the workers


    char *userCommand;    
    userCommand = clientReadCommands();                     //Get a command from the user
    while(strcmp(userCommand,"exit") != 0){                 //While user command is not exit

        if(childInteruption){                               //If a child was interupted previously
            pid_t childPid = wait(NULL);

            int i = 0;
            while(waitId[i] != childPid){
                i++;
            }

            if(childPid == waitId[i]){

                char processNumber[12];
                sprintf(processNumber,"%d",i);

                char *args[] = {"./worker",processNumber,NULL};
                if((waitId[i] = fork()) == 0){
                    execvp(args[0],args);                                       //Child processes become workers
                }
            
                char *serverResponse = NULL;

                while((serverResponse == NULL) || (strcmp(serverResponse,"!") != 0)){

                    free(serverResponse);

                    poll(&serverFdArray[i], 1, -1);
                    if(serverFdArray[i].revents & POLLIN){
                        namedPipeRead(serverFd[i],&serverResponse);
                    }
                }
                free(serverResponse);

                stringConcat(&clientNamedPipe,clientNamedPipePath,processNumber);
                
                int size = strlen("fileReading") + 1;

                clientFd[i] = open(clientNamedPipe, O_WRONLY | O_NONBLOCK);         //Open the current named pipe
                write(clientFd[i],&size,sizeof(int));                               //Write in the named pipe
                write(clientFd[i], "fileReading",size);
                close(clientFd[i]);                                                 //Close the current named pipe

                serverDistributeCountries(countryDirList,numOfWorkers,clientNamedPipePath,i);
                
                size = strlen("!") + 1;

                clientFd[i] = open(clientNamedPipe, O_WRONLY | O_NONBLOCK);         //Open the current named pipe
                write(clientFd[i],&size,sizeof(int));                               //Write in the named pipe
                write(clientFd[i], "!",size);
                close(clientFd[i]);                                                 //Close the current named pipe
            
                free(clientNamedPipe);
            
                serverResponse = NULL;

                while((serverResponse == NULL) || (strcmp(serverResponse,"!") != 0)){

                    free(serverResponse);

                    poll(&serverFdArray[i], 1, -1);
                    if(serverFdArray[i].revents & POLLIN){
                        namedPipeRead(serverFd[i],&serverResponse);
                    }
                }
                free(serverResponse);
            }           

            childInteruption = 0;
        }

        signal(SIGINT,interuptAggregator);
        signal(SIGQUIT,interuptAggregator);

        clientWriteToServer(numOfWorkers,clientNamedPipePath,userCommand);          //Write command to workers

        clientReadFromServer(numOfWorkers,serverFdArray,serverFd,&logStatistics);   //Read results from workers

        if(aggregatorInteruptionSignal){
            break;
        }

        free(userCommand);
        userCommand = clientReadCommands();                                         //Read a new user command
    }
    free(userCommand);

    clientWriteToServer(numOfWorkers,clientNamedPipePath,"logStatistics");

    clientReadFromServer(numOfWorkers,serverFdArray,serverFd,&logStatistics);

    for(int i = 0; i < numOfWorkers; i++){                      //Exit has been given
        
        close(serverFd[i]);                                     //Close all open diseaseAggregator named pipes
        kill(waitId[i], SIGKILL);                               //Send SIGKILL signal to all children
        wait(NULL);                                             //Wait children to terminate
    }
    
    freeList(countryDirList);
    free(inputDir);
    return 0;
}