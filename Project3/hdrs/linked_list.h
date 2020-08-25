#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <string.h>
#include "patientRecords.h"
#include "date.h"

typedef struct List_Node{

    void *payload;
    struct List_Node *next;
}listNode;

typedef struct List{

    int length;
    listNode *first;
    listNode *last;
}List;

/*************************************************
 *                                               *
 *                  Initializations              *
 *                                               *
 *************************************************/

//Initialize A List Node
void initializeListNode(listNode **node, void *payload);
//Initialize A List
void initializeList(List **list);

/**************************************************
 *                                                *
 *                      Utilities                 *
 *                                                *
 **************************************************/

//Get A Node's Payload
void * getPayload(listNode *node);

//Get First Node Of List
listNode *getFirst(List *list);

//Remove the LastNode from the list
listNode *removeFirst(List *list);

//Get Last Node Of List
listNode *getLast(List *list);

//Get Length Of List
int length(List *list);

//Insert An Element First On The List
void insertFirst(List *list, listNode *node);

//Insert An Element Last On The List
void insertLast(List *list, listNode *node);

//Insert An Element In Alphabetical order
void insertOrderdString(List *list, listNode *node);

//Insert A FilePath Ordered By Date
void insertOrderdDate(List *list, listNode *node);

//Count patientRecords of given country
int countByCountry(List* list, char *country);

//Count patientRecords of given disease
int countByDisease(List *list, char *disease);

//Count patientRecords entered or exited at a given date
int countByDate(List *list, char *date);

//Count patientRecords on a given age range
int countByAgeAndCountry(List *list, char *ageCountry);

//Count patientRecords with given disease in given age range
int countByAgeAndDisease(List *list, char *diseaseAndAge);

//Count exited patientRecords from given country
int countExitedFromCountry(List *list, char *country);

//Count exited patientRecords with given disease
int countExitedWithDisease(List *list, char *disease);

//Count patientRecords with given disease and country in given age range
int countByAgeAndDiseaseAndCountry(List *list, char *diseaseAndCountryAndAge);

//Check If Patients Recrod Exists In List
patientRecord *findRecord(List *list, char *id);

//Check If List Is Empty
int isEmpty(List *list);

//Find string in list
listNode* stringExistsInList(List *list, char *element);

//Find Element In List
listNode* findElement(List *list, void *element);

//Find Records Of Patients Without Exit Date
int currentPatientsInList(List *list);

//Print List
void printList(List *l);

/***************************************************
 *                                                 *
 *                  Destruction                    *
 *                                                 *
 ***************************************************/

//Free A List Node
void freeListNode(listNode *node);
//Free A List
void freeList(List* list);

//Free A List Node But Dont Free Values
void freeNonValueListNode(listNode *node);
//Free A List But Dont Free Values
void freeNonValueList(List* list);

//Free A Patient Records List Node
void freePatientRecordsListNode(listNode *node);
//Free A Patient Records List
void freePatientRecordsList(List *list);
#endif