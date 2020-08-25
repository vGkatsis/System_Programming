#ifndef FD_TYPE_H
#define FD_TYPE_H

#include <stdio.h>

//Struct representing a set of fileDescriptor and type of fileDescriptor
typedef struct FdType{

    int fd;
    char type;

}fdType;

/**************************************
 *                                    *
 *          Initialization            *
 *                                    *
 **************************************/

//Initialize an fdType struct
void initFdType(fdType *fdType);

/**************************************
 *                                    *
 *               Utilities            *
 *                                    *
 **************************************/

//Check if this file descriptor belongs to a worker process
int fdIsWorker(fdType *fdType);

//Check if this file descriptor belogs to whoClient
int fdIsWhoClient(fdType *fdType);

//Check fdType struct has been set;
int fdTypeIsSet(fdType *fdType);

/**************************************
 *                                    *
 *          Getters/Setters           *
 *                                    *
 **************************************/

//Get the file descriptor
int getFd(fdType *fdType);

//Get the type of the file descriptor
char getFdType(fdType *fdType);

//Set the file descriptor
void setFd(fdType *fdType, int newFd);

//Set the type of the file descriptor
void setFdType(fdType *fdType, char newType);
#endif