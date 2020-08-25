#include <stdio.h>
#include <stdlib.h>
#include "../headers/linked_list.h"
#include "../headers/hashTable.h"
#include "../headers/error_handler.h"
#include "../headers/fileReading.h"
#include "../headers/commands.h"

int main(int argc, char **argv){

    FILE *patientRecordsFile = NULL;
    hashTable *countryHashTable,*diseaseHashTable, *recordHashTable;
    int diseaseHashNum, countryHashNum, bucketSize;

    if(!argCheck(argc,argv,&diseaseHashNum,&countryHashNum,&bucketSize,&patientRecordsFile)){       //Check if program has been called correctly
        if(patientRecordsFile != NULL){
            fclose(patientRecordsFile);
        }
        return 1;
    }

//    printf("Reading data from file...\n");
    if(readPatientRecords(patientRecordsFile,&recordHashTable,&countryHashTable,countryHashNum,
                                                &diseaseHashTable,diseaseHashNum,bucketSize)){       //Read data and fill data structures
        fclose(patientRecordsFile);
        printf("Done Reading.\n");

        readCommands(recordHashTable,countryHashTable,diseaseHashTable,bucketSize);                      //Main program functionality
    }else{
        fclose(patientRecordsFile);
    }
    

//    printf("\nCleaning Data...\n");                                                                  //Free all allocated memmory
    printf("exiting\n");

    freeHashTable(recordHashTable,bucketSize,"record");
    freeHashTable(countryHashTable,bucketSize,"tree");
    freeHashTable(diseaseHashTable, bucketSize,"tree");

    return 0;
}
