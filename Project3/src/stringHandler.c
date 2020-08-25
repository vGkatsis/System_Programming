#include "../hdrs/stringHandler.h"

void stringConcat(char **finalString, char *firstString, char *lastString){

     //Allocate necesary space for final string
    (*finalString) = (char *)malloc((strlen(firstString) + strlen(lastString) + 1) * sizeof(char));

    //Concatenate empty string with first string
    strcpy((*finalString),firstString);
    //Concatenate first string with last string
    strcat((*finalString),lastString);
}

void addSlashAtEnd(char **finalString){

    if((*finalString)[strlen((*finalString))-1] != '/'){

        if(((*finalString) = (char *) realloc((*finalString),(strlen((*finalString)) + strlen("/") + 1) * sizeof(char))) == NULL){
            perror("Error while reallocating space in addSlashAtEnd\n");
            exit(11);
        }

        strcat((*finalString),"/");
    }

    return;
}

void partStringToChunks();