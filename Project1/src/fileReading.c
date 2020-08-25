#include "../headers/fileReading.h"

//Function reading patient records file
int readPatientRecords(FILE *patientRecords, hashTable **records,hashTable **countryHashTable,int countryHashNum,
                                                    hashTable **diseaseHashTable,int diseaseHashNum, int bucketSize){

    int numberOfLines = 0;
    char *line, *option, *value;
    size_t size = 1;
    patientRecord *pRecord;

    //Initialize disease and country hash tables
    initializeHashTable(countryHashTable,countryHashNum);
    initializeHashTable(diseaseHashTable,diseaseHashNum);

    //Count the number of lines of the reading file
    while(!feof(patientRecords))
    {
        char ch = fgetc(patientRecords);
        if(ch == '\n'){
            numberOfLines++;
        }
    }
    rewind(patientRecords);

    //Initialize the hashTable to hold patient records
    numberOfLines /= 4;
    initializeHashTable(records,numberOfLines);

    if(((line = (char*) malloc(size * sizeof(char)))) == NULL){
        perror("\nError While Allocating Memmory For File Reading\n");
        exit(2);
    }

    //Read patient record data
    while(getline(&line,&size,patientRecords) != -1){
        
        initializePatientRecord(&pRecord);

        option = strtok(line, " ");                                                             //Gather All Given Information
        if((value = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
            perror("\nError while allocating space for recordId\n");
            exit(2);
        }
        strcpy(value,option);
        setRecordId(pRecord,value);

        //If the recordId has already been given abort file reading
        if(findRecordInHash((*records),value,bucketSize) != NULL){
//            printf("\nRecord with recordId \"%s\" is a duplicate entry...\n",value);
//            printf("Ending application\n");
            printf("error\n");            
            freePatientRecord(pRecord);
            free(line);
            return 0;
        }
    
        option = strtok(NULL, " ");
        if((value = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
            perror("\nError while allocating space for FirstName\n");
            exit(2);
        }
        strcpy(value,option);
        setFirstName(pRecord,value);
    
        option = strtok(NULL, " ");
        if((value = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
            perror("\nError while allocating space for LastName\n");
            exit(2);
        }
        strcpy(value,option);
        setLastName(pRecord,value);
    
        option = strtok(NULL, " ");
        if((value = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
            perror("\nError while allocating space for DiseaseID\n");
            exit(2);
        }
        strcpy(value,option);
        setDiseaseID(pRecord,value);
    
        option = strtok(NULL, " ");
        if((value = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
            perror("\nError while allocating space for Country\n");
            exit(2);
        }
        strcpy(value,option);
        setCountry(pRecord,value);
    
        option = strtok(NULL, " ");
        if((value = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
            perror("\nError while allocating space for Entry Date\n");
            exit(2);
        }
        strcpy(value,option);
        setEntryDate(pRecord,value);
    
        option = strtok(NULL, " ");
        if((value = (char *) malloc((strlen(option) + 1) * sizeof(char))) == NULL){
            perror("\nError while allocating space for ExitDate\n");
            exit(2);
        }
        strcpy(value,option);
        setExitDate(pRecord,value);

        if((pRecord->exitDate)[0] != '-'){

            date Entry, Exit;

            //If exit date is previous than entry date reject the record
            parseDate(pRecord->entryDate,&Entry);
            parseDate(pRecord->exitDate,&Exit);
            if(compareDates(&Entry,&Exit) > 0){
//                printf("\nExit Date can not be previous than Entry Date...\n");
//                printf("PatientRecord insertion rejected\n");
                printf("error\n");              
                freePatientRecord(pRecord);
                continue;
            }
        }

        insertElementInHash((*records),pRecord->recordId,pRecord,bucketSize);                        //Insert Record In Record HashTable
        insertRecordInHash((*countryHashTable),pRecord->Country,pRecord,bucketSize);                     //Insert Record In Country HashTable
        insertRecordInHash((*diseaseHashTable),pRecord->diseaseId,pRecord,bucketSize);                   //Insert Record In Disease HashTable
    }


    free(line);
    return 1;
}