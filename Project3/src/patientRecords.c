#include "../hdrs/patientRecords.h"

void initializePatientRecord(patientRecord **record){

    if(((*record) = (patientRecord *) malloc(1 * sizeof(patientRecord))) == NULL){

        perror("\nError while allocating space for a record\n");
        exit(3);
    }

    (*record)->diseaseId = NULL;
    (*record)->patientFirstName = NULL;
    (*record)->patientLastName = NULL;
    (*record)->recordId = NULL;
    (*record)->Country = NULL;
    (*record)->entryDate = NULL;
    (*record)->exitDate = NULL;
    (*record)->Age = 0;
}

void freePatientRecord(patientRecord *record){

    free(record->recordId);
    free(record->patientFirstName);
    free(record->patientLastName);
    free(record->Country);
    free(record->diseaseId);
    free(record->entryDate);
    free(record->exitDate);

    free(record);
}

char * getRecordId(patientRecord *record){
    return record->recordId;
}

char * getFirstName(patientRecord *record){
    return record->patientFirstName;
}

char * getLastName(patientRecord *record){
    return record->patientLastName;
}

char * getDiseaseID(patientRecord *record){
    return record->diseaseId;
}

char * getCountry(patientRecord *record){
    return record->Country;
}

char * getEntryDate(patientRecord *record){
    return record->entryDate;
}

char * getExitDate(patientRecord *record){
    return record->exitDate;
}

int getAge(patientRecord *record){
    return record->Age;
}

void setRecordId(patientRecord *record, char *newRecordID){
    record->recordId = newRecordID;
}

void setFirstName(patientRecord *record, char *newFirstName){
    record->patientFirstName = newFirstName;
}

void setLastName(patientRecord *record, char *newLastName){
    record->patientLastName = newLastName;
}

void setDiseaseID(patientRecord *record, char *newDiseaseId){
    record->diseaseId = newDiseaseId;
}

void setCountry(patientRecord *record, char *newCountry){
    record->Country = newCountry;
}

void setEntryDate(patientRecord *record, char *newEntryDate){
    record->entryDate = newEntryDate;
}

void setExitDate(patientRecord *record, char *newExitDate){
    record->exitDate = newExitDate;
}

void setAge(patientRecord *record, int newAge){
    record->Age = newAge;
}

void printPatientRecords(patientRecord *record){

    printf("Record id %s \t",getRecordId(record));
    printf("First Name %s \t",getFirstName(record));
    printf("Last Name %s \t",getLastName(record));
    printf("Country %s \t",getCountry(record));
    printf("Disease Id %s \t",getDiseaseID(record));
    printf("Entry Date %s \t",getEntryDate(record));
    printf("Exit Date %s \n",getExitDate(record));
}