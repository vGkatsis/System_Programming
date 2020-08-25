#ifndef PATIENT_RECORDS_H
#define PATIENT_RECORDS_H

#include <stdlib.h>
#include <stdio.h>

typedef struct PatientRecord{

    char *recordId;
    char *patientFirstName;
    char *patientLastName;
    char *diseaseId;
    char *Country;
    char *entryDate;
    char *exitDate;
    int  Age;
}patientRecord;

/************************************
 *                                  *
 *   Initialization/Destruction     *
 *                                  *
 ************************************/

void initializePatientRecord(patientRecord **record);
void freePatientRecord(patientRecord *record);

/************************************
 *                                  *
 *              Getters             *
 *                                  *
 ************************************/

char * getRecordId(patientRecord *record);
char * getFirstName(patientRecord *record);
char * getLastName(patientRecord *record);
char * getDiseaseID(patientRecord *record);
char * getCountry(patientRecord *record);
char * getEntryDate(patientRecord *record);
char * getExitDate(patientRecord *record);
int    getAge(patientRecord *record);

/************************************
 *                                  *
 *              Setters             *
 *                                  *
 ************************************/

void setRecordId(patientRecord *record, char *newRecordID);
void setFirstName(patientRecord *record, char *newFirstName);
void setLastName(patientRecord *record, char *newLastName);
void setDiseaseID(patientRecord *record, char *newDiseaseId);
void setCountry(patientRecord *record, char *newCountry);
void setAge(patientRecord *record, int newAge);
void setEntryDate(patientRecord *record, char *newEntryDate);
void setExitDate(patientRecord *record, char *newExitDate);

/************************************
 *                                  *
 *            Utilities             *
 *                                  *
 ************************************/

void printPatientRecords(patientRecord *record);

#endif