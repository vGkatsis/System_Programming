#include "../hdrs/fdType.h"

//Initialize an fdType struct
void initFdType(fdType *fdType){

    fdType->fd = -1;
    fdType->type = 'u';
}

//Check if this file descriptor belongs to a worker process
int fdIsWorker(fdType *fdType){

    if(fdType->type == 'w'){
        return 1;
    }
    return 0;
}

//Check if this file descriptor belogs to whoClient
int fdIsWhoClient(fdType *fdType){

    if(fdType->type == 'c'){
        return 1;
    }
    return 0;
}

//Check fdType struct has been set;
int fdTypeIsSet(fdType *fdType){

    if(fdType->type == 'u' && fdType->fd == -1){
        return 1;
    }

    printf("FdType struct has been partialy or not initialized\n");
    return 0;
}

//Get the file descriptor
int getFd(fdType *fdType){
    return fdType->fd;
}

//Get the type of the file descriptor
char getFdType(fdType *fdType){
    return fdType->type;
}

//Set the file descriptor
void setFd(fdType *fdType, int newFd){
    fdType->fd = newFd;
}

//Set the type of the file descriptor
void setFdType(fdType *fdType, char newType){
    fdType->type = newType;
}