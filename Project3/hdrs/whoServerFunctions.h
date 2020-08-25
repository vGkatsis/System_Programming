#ifndef WHO_SERVER_FUNCTIONS_H
#define WHO_SERVER_FUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <poll.h>
#include "../hdrs/circularBuffer.h"
#include "../hdrs/communication.h"

extern pthread_mutex_t startServerThread;
extern pthread_mutex_t readBufferMutex;
extern pthread_mutex_t modifyWorkersNum;
extern pthread_mutex_t addWorkerPort;
extern pthread_mutex_t executeQuery;
extern pthread_cond_t serverThreadReady;


//Server thread functionality
void *threadServerCommand(void *arg);

#endif