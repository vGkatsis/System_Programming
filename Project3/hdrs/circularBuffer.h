#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../hdrs/fdType.h"

//Struct representing a curcular buffer
typedef struct CircularBuffer{

    fdType *buffer;
    int first;
    int last;
    int bufferSize;
}circularBuffer;

/*************************************
 *                                   *
 *     Initialization/Destruction    *
 *                                   *
 *************************************/

//Initialize a circular buffer
void initializeCircular(circularBuffer *buffer, int bufferSize);

//Free a circular buffer
void freeCircular(circularBuffer *buffer);

/*************************************
 *                                   *
 *              Utilities            *
 *                                   *
 *************************************/

//Insert an element in a circular buffer
int insertInCircular(circularBuffer *buffer, int fd, char type);

//Remove an element from a circular buffer
fdType removeFromCircular(circularBuffer *buffer);

//Check if circular buffer is empty
int circularIsEmpty(circularBuffer *buffer);

//Check if circular buffer is empty
int circularIsFull(circularBuffer *buffer);

/**************************************
 *                                    *
 *          Getters/Setters           *
 *                                    *
 **************************************/

//Get the circular buffer
fdType *getBuffer(circularBuffer *circular);

//Get the first position in the circular buffer
int getCircularFirst(circularBuffer *buffer);

//Get the last position in the circular buffer
int getCircularLast(circularBuffer *buffer);

//Get the buffer size of the circular buffer
int getCircularBufferSize(circularBuffer *buffer);

//Set the circular buffer
void setCircularBuffer(circularBuffer *buffer, fdType *newBuffer);

//Set the first position in the circular buffer
void setCircularFirst(circularBuffer *buffer, int newFirst);

//Set the last position in the circular buffer
void setCircularLast(circularBuffer *buffer, int newLast);

//Set the buffer size of the circular buffer
void setCircularBufferSize(circularBuffer *buffer, int newBufferSize);

#endif