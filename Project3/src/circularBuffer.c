#include "../hdrs/circularBuffer.h"

//Initialize a circular buffer
void initializeCircular(circularBuffer *buffer, int bufferSize){
    
    if((buffer->buffer = (fdType *) malloc(bufferSize * sizeof(fdType))) == NULL){
        perror("Error while allocating space for circular buffer\n");
        exit(15);
    }

    for(int i = 0; i < bufferSize; i++){
        initFdType(&(buffer->buffer[i]));
    }

    buffer->first = -1;
    buffer->last = -1;
    buffer->bufferSize = bufferSize;
}

//Free a circular buffer
void freeCircular(circularBuffer *buffer){

    free(buffer->buffer);
}

//Insert an element in a circular buffer
int insertInCircular(circularBuffer *buffer, int fd, char type){

    if(circularIsFull(buffer)){
        return -1;
    }

    if(buffer->last == (buffer->bufferSize - 1)){

        buffer->last = 0;
    }else{

        buffer->last++;
    }

    buffer->buffer[buffer->last].fd = fd;
    buffer->buffer[buffer->last].type = type;

    return buffer->last;
}

//Remove an element from a circular buffer
fdType removeFromCircular(circularBuffer *buffer){

    if(circularIsEmpty(buffer)){
        fdType emptyFdType;
        
        initFdType(&emptyFdType);
        return emptyFdType;
    }

    if(buffer->first == (buffer->bufferSize - 1)){

        buffer->first = 0;
    }else{

        buffer->first++;
    }

    fdType firstElement = buffer->buffer[buffer->first];
    initFdType(&(buffer->buffer[buffer->first]));

    if(circularIsEmpty(buffer)){
        buffer->first = -1;
        buffer->last = -1;
    }

    return firstElement;
}

//Check if circular buffer is empty
int circularIsEmpty(circularBuffer *buffer){
    
    if(((buffer->first == -1) && (buffer->last == -1)) ||
        ((buffer->first == buffer->last) && (buffer->buffer[buffer->first].fd == -1))){
        return 1;
    }

    return 0;
}

//Check if circular buffer is empty
int circularIsFull(circularBuffer *buffer){

    if(((buffer->last - buffer->first) == buffer->bufferSize) ||
        (((buffer->first != -1) && (buffer->last != -1)) && (buffer->first == buffer->last) && (buffer->buffer[buffer->first].fd != -1))){

        return 1;
    }

    return 0;
}

//Get the circular buffer
fdType *getBuffer(circularBuffer *circular){
    return circular->buffer;
}

//Get the first element in the circular buffer
int getCircularFirst(circularBuffer *buffer){
    return buffer->first;
}

//Get the last element in the circular buffer
int getCircularLast(circularBuffer *buffer){
    return buffer->last;
}

//Get the buffer size of the circular buffer
int getCircularBufferSize(circularBuffer *buffer){
    return buffer->bufferSize;
}

//Set the circular buffer
void setCircularBuffer(circularBuffer *buffer, fdType *newBuffer){
    buffer->buffer = newBuffer;
}

//Set the first position in the circular buffer
void setCircularFirst(circularBuffer *buffer, int newFirst){
    buffer->first = newFirst;
}

//Set the last position in the circular buffer
void setCircularLast(circularBuffer *buffer, int newLast){
    buffer->last = newLast;
}

//Set the buffer size of the circular buffer
void setCircularBufferSize(circularBuffer *buffer, int newBufferSize){
    buffer->bufferSize = newBufferSize;
}