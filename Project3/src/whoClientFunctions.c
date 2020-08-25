#include "../hdrs/whoClientFunctions.h"

pthread_mutex_t startMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t writeMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t readyCond = PTHREAD_COND_INITIALIZER;

void *threadReadCommand(void *arg){

    socketCommand *socketCommandPair = (socketCommand *)arg;
    
    pthread_mutex_lock(&startMutex);

    pthread_cond_wait(&readyCond, &startMutex);

    pthread_mutex_unlock(&startMutex);

    printf("Sending %s line...\n",socketCommandPair->command);
    int lineSize = strlen(socketCommandPair->command);

    pthread_mutex_lock(&writeMutex);

    write(socketCommandPair->socket,&lineSize,sizeof(int));
    write(socketCommandPair->socket,socketCommandPair->command, lineSize);

    pthread_mutex_unlock(&writeMutex);
}