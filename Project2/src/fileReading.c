#include "../hdrs/fileReading.h"

//Function reading patient records file
int readPatientRecords(FILE *patientRecords, hashTable *records,hashTable *countryHashTable,
                                    hashTable *diseaseHashTable, int bucketSize, char *country, char *currentDate, logStats *logStatistics){

    int numberOfLines = 0;
    char *line, *option;
    size_t size = 1;
    patientRecord *pRecord;
    date fileDate;

    if(((line = (char*) malloc(size * sizeof(char)))) == NULL){
        perror("\nError While Allocating Memmory For File Reading\n");
        exit(2);
    }

    parseDate(currentDate,&fileDate);

    //Read patient record data
    while(getline(&line,&size,patientRecords) != -1){
        initializePatientRecord(&pRecord);

        option = strtok(line, " ");
        if(option != NULL){
            if((pRecord->recordId = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
                perror("\nError while allocating space for recordId\n");
                exit(2);
            }
            strcpy(pRecord->recordId,option);
        }else{
            fprintf(stderr,"ERROR\n");
            freePatientRecord(pRecord);
            continue;
        }

        patientRecord *existingRecord;

        option = strtok(NULL, " ");
        if(option == NULL){
            fprintf(stderr,"ERROR\n");
            freePatientRecord(pRecord);
            continue;
        }
        
        if(strcmp(option,"EXIT") == 0){

            //If the recordId does not exist abort file reading
            if((existingRecord = findRecordInHash(records,pRecord->recordId,bucketSize)) == NULL){
//                printf("\nRecord with recordId \"%s\" does not exist\n",value);
                fprintf(stderr,"ERROR\n");            
                freePatientRecord(pRecord);
                continue;
            }

            if((existingRecord->exitDate)[0] == '-'){

                date Entry;
                //If exit date is previous than entry date reject the record
                parseDate(existingRecord->entryDate,&Entry);
                if(compareDates(&Entry,&fileDate) > 0){
//                    printf("\nExit Date can not be previous than Entry Date...\n");
//                    printf("PatientRecord insertion rejected\n");
                    fprintf(stderr,"ERROR\n");              
                    freePatientRecord(pRecord);
                    continue;
                }
            
                free(existingRecord->exitDate);
                if((existingRecord->exitDate = (char *) malloc((strlen(currentDate) + 1) * sizeof(char))) == NULL){
                    perror("\nError while allocating space for CurrentDate\n");
                    exit(2);
                }
                strcpy(existingRecord->exitDate,currentDate);
            }
            freePatientRecord(pRecord);
            continue;
        }else{

            //If the recordId exists abort file reading
            if((existingRecord = findRecordInHash(records,pRecord->recordId,bucketSize)) != NULL){
//                printf("\nRecord with recordId \"%s\" exists\n",value);
                fprintf(stderr,"ERROR\n");            
                freePatientRecord(pRecord);
                continue;
            }

            if((pRecord->entryDate = (char *) malloc((strlen(currentDate) + 1) * sizeof(char))) == NULL){
                perror("\nError while allocating space for CurrentDate\n");
                exit(2);
            }
            strcpy(pRecord->entryDate,currentDate);
            
            if((pRecord->exitDate = (char *) malloc((strlen("-") + 1) * sizeof(char))) == NULL){
                perror("\nError while allocating space for CurrentDate\n");
                exit(2);
            }
            strcpy(pRecord->exitDate,"-");
        }

        option = strtok(NULL, " ");
        if(option == NULL){
            fprintf(stderr,"ERROR\n");
            freePatientRecord(pRecord);
            continue;
        }
        if((pRecord->patientFirstName = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
            perror("\nError while allocating space for FirstName\n");
            exit(2);
        }
        strcpy(pRecord->patientFirstName,option);
    
        option = strtok(NULL, " ");
        if(option == NULL){
            fprintf(stderr,"ERROR\n");
            freePatientRecord(pRecord);
            continue;
        }
        if((pRecord->patientLastName = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
            perror("\nError while allocating space for LastName\n");
            exit(2);
        }
        strcpy(pRecord->patientLastName,option);
    
        option = strtok(NULL, " ");
        if(option == NULL){
            fprintf(stderr,"ERROR\n");
            freePatientRecord(pRecord);
            continue;
        }
        if((pRecord->diseaseId = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
            perror("\nError while allocating space for DiseaseID\n");
            exit(2);
        }
        strcpy(pRecord->diseaseId,option);
    
        if((pRecord->Country = (char *) malloc((strlen(country) + 1) * sizeof(char))) == NULL){
            perror("\nError while allocating space for country\n");
            exit(2);
        }
        strcpy(pRecord->Country,country);

        option = strtok(NULL, " ");
        if(option == NULL){
            fprintf(stderr,"ERROR\n");
            freePatientRecord(pRecord);
            continue;
        }
        setAge(pRecord,atoi(option));


        insertElementInHash(records,pRecord->recordId,pRecord,bucketSize);                        //Insert Record In Record HashTable
        insertRecordInHash(countryHashTable,pRecord->Country,pRecord,bucketSize);                 //Insert Record In Country HashTable
        insertRecordInHash(diseaseHashTable,pRecord->diseaseId,pRecord,bucketSize);               //Insert Record In Disease HashTable
    }

    free(line);
    incrementTotalRequests(logStatistics);
    incrementSuccessRequests(logStatistics);
    return 1;
}