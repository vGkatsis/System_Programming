#ifndef WHO_CLIENT_FUNCTIONS_H
#define WHO_CLIENT_FUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>

extern pthread_mutex_t startMutex;
extern pthread_mutex_t writeMutex;
extern pthread_cond_t readyCond;

typedef struct SocketCommand{

    int socket;
    char *command;
}socketCommand;


//Client thread functionality
void *threadReadCommand(void *arg);

#endif