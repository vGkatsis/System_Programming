#include <stdio.h>
#include "../hdrs/circularBuffer.h"

int main(void){

    circularBuffer buffer;

    initializeCircular(&buffer,3);

    removeFromCircular(&buffer);

    printf("Result should be:\n");
    printf("Position 0 and fd -1 and type u\n");
    printf("Position 1 and fd -1 and type u\n");
    printf("Position 2 and fd -1 and type u\n");

    printf("\n");

    for(int i = 0; i < buffer.bufferSize; i++){
        printf("Position %d and fd %d and type %c\n",i,buffer.buffer[i].fd,buffer.buffer[i].type);
    }

    printf("\n");

    printf("Result should be:\n");
    printf("Position 0 and fd -1 and type u\n");
    printf("Position 1 and fd -1 and type u\n");
    printf("Position 2 and fd -1 and type u\n");

    printf("\n");

    for(int i = 0; i < buffer.bufferSize; i++){
        printf("Position %d and fd %d and type %c\n",i,buffer.buffer[i].fd,buffer.buffer[i].type);
    }

    printf("\n");

    insertInCircular(&buffer,1,'w');

    printf("Result should be:\n");
    printf("Position 0 and fd 1 and type w\n");
    printf("Position 1 and fd -1 and type u\n");
    printf("Position 2 and fd -1 and type u\n");
    
    printf("\n");

    for(int i = 0; i < buffer.bufferSize; i++){
        printf("Position %d and fd %d and type %c\n",i,buffer.buffer[i].fd,buffer.buffer[i].type);
    }
    
    removeFromCircular(&buffer);

    printf("Result should be:\n");
    printf("Position 0 and fd -1 and type u\n");
    printf("Position 1 and fd -1 and type u\n");
    printf("Position 2 and fd -1 and type u\n");
    
    printf("\n");

    for(int i = 0; i < buffer.bufferSize; i++){
        printf("Position %d and fd %d and type %c\n",i,buffer.buffer[i].fd,buffer.buffer[i].type);
    }

    insertInCircular(&buffer,1,'w');
    insertInCircular(&buffer,2,'c');
    insertInCircular(&buffer,3,'c');

    printf("\n");

    printf("Result should be:\n");
    printf("Position 0 and fd 1 and type w\n");
    printf("Position 1 and fd 2 and type c\n");
    printf("Position 2 and fd 3 and type c\n");
    
    printf("\n");

    for(int i = 0; i < buffer.bufferSize; i++){
        printf("Position %d and fd %d and type %c\n",i,buffer.buffer[i].fd,buffer.buffer[i].type);
    }

    insertInCircular(&buffer,4,'w');

    printf("\n");

    printf("Result should be:\n");
    printf("Position 0 and fd 1 and type w\n");
    printf("Position 1 and fd 2 and type c\n");
    printf("Position 2 and fd 3 and type c\n");
    
    printf("\n");

    for(int i = 0; i < buffer.bufferSize; i++){
        printf("Position %d and fd %d and type %c\n",i,buffer.buffer[i].fd,buffer.buffer[i].type);
    }

    removeFromCircular(&buffer);

    printf("\n");

    printf("Result should be:\n");
    printf("Position 0 and fd -1 and type u\n");;
    printf("Position 1 and fd 2 and type c\n");
    printf("Position 2 and fd 3 and type c\n");
    
    printf("\n");

    for(int i = 0; i < buffer.bufferSize; i++){
        printf("Position %d and fd %d and type %c\n",i,buffer.buffer[i].fd,buffer.buffer[i].type);
    }

    insertInCircular(&buffer,4,'w');

    printf("\n");

    printf("Result should be:\n");
    printf("Position 0 and fd 4 and type w\n");
    printf("Position 1 and fd 2 and type c\n");
    printf("Position 2 and fd 3 and type c\n");
    
    printf("\n");

    for(int i = 0; i < buffer.bufferSize; i++){
        printf("Position %d and fd %d and type %c\n",i,buffer.buffer[i].fd,buffer.buffer[i].type);
    }

    removeFromCircular(&buffer);

    printf("\n");

    printf("Result should be:\n");
    printf("Position 0 and fd 4 and type w\n");
    printf("Position 1 and fd -1 and type u\n");
    printf("Position 2 and fd 3 and type c\n");
    
    printf("\n");

    for(int i = 0; i < buffer.bufferSize; i++){
        printf("Position %d and fd %d and type %c\n",i,buffer.buffer[i].fd,buffer.buffer[i].type);
    }

    removeFromCircular(&buffer);

    printf("\n");

    printf("Result should be:\n");
    printf("Position 0 and fd 4 and type w\n");
    printf("Position 1 and fd -1 and type u\n");
    printf("Position 2 and fd -1 and type u\n");
    
    printf("\n");

    for(int i = 0; i < buffer.bufferSize; i++){
        printf("Position %d and fd %d and type %c\n",i,buffer.buffer[i].fd,buffer.buffer[i].type);
    }

    insertInCircular(&buffer,5,'c');

    printf("\n");

    printf("Result should be:\n");
    printf("Position 0 and fd 4 and type w\n");
    printf("Position 1 and fd 5 and type c\n");
    printf("Position 2 and fd -1 and type u\n");
     
   printf("\n");

    for(int i = 0; i < buffer.bufferSize; i++){
        printf("Position %d and fd %d and type %c\n",i,buffer.buffer[i].fd,buffer.buffer[i].type);
    }

    insertInCircular(&buffer,6,'w');

    printf("\n");

    printf("Result should be:\n");
    printf("Position 0 and fd 4 and type w\n");
    printf("Position 1 and fd 5 and type c\n");
    printf("Position 2 and fd 6 and type w\n");
    
    printf("\n");

    for(int i = 0; i < buffer.bufferSize; i++){
        printf("Position %d and fd %d and type %c\n",i,buffer.buffer[i].fd,buffer.buffer[i].type);
    }

    insertInCircular(&buffer,7,'w');

    printf("\n");

    printf("Result should be:\n");
    printf("Position 0 and fd 4 and type w\n");
    printf("Position 1 and fd 5 and type c\n");
    printf("Position 2 and fd 6 and type w\n");
    
    printf("\n");

    for(int i = 0; i < buffer.bufferSize; i++){
        printf("Position %d and fd %d and type %c\n",i,buffer.buffer[i].fd,buffer.buffer[i].type);
    }

    freeCircular(&buffer);
}