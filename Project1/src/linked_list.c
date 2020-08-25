#include <stdio.h>
#include <stdlib.h>
#include "../headers/linked_list.h"
#include "../headers/patientRecords.h"

//Initialize A List Node
void initializeListNode(listNode **node, void *payload){

    if(((*node) = (listNode *) malloc(1 * sizeof(listNode))) == NULL){
        perror("\nError while allocating space for list node\n");
        exit(1);
    }

    (*node)->payload = payload;
    (*node)->next = NULL;
}

//Initialize A List
void initializeList(List **list){

    if(((*list) = (List *) malloc(1 * sizeof(List))) == NULL){
        perror("\nList initialization FAILED\n");
        exit(1);
    }

    (*list)->length = 0;
    (*list)->first = NULL;
    (*list)->last = NULL;
}

//Get A Node's Payload
void * getPayload(listNode *node){
    return node->payload;
}

//Get First Node Of List
listNode *getFirst(List *list){
    return list->first;
}

//Remove the LastNode from the list
listNode *removeFirst(List *list){

    listNode *tmpFirst = list->first;

    list->first = list->first->next;
    
    if(list->first == NULL){
        list->last = NULL;
    }
    list->length--;

    return tmpFirst;
}

//Get Last Node Of List
listNode *getLast(List *list){
    return list->last;
}

//Get Length Of List
int length(List *list){
    return list->length;
}

//Insert An Element First On The List
void insertFirst(List *list, listNode *node){

    if(list == NULL){
        initializeList(&list);
    }

    if(list->first == NULL){
        list->first = node;
        list->last = node;
    }else{

        listNode *tmp = list->first;
        list->first = node;
        list->first->next = tmp;
    }

    list->length++;
}

//Insert An Element Last On The List
void insertLast(List *list, listNode *node){

    if(list == NULL){
        initializeList(&list);
    }

    if(list->first == NULL){
        list->first = node;
        list->last = node;
    }else{

        listNode *tmp = list->last;
        list->last = node;
        tmp->next = list->last;
    }

    list->length++;
}

//Insert PatientRecords In Alphabetical order(based on recordId)
void insertOrderdString(List *list, listNode *node){

    if((list == NULL) || (list->first == NULL)){
        insertFirst(list,node);
        return;
    }

    patientRecord *nodeRecord, *firstRecord, *lastRecord;

    nodeRecord = (patientRecord *)node->payload;
    firstRecord = (patientRecord *)list->first->payload;
    lastRecord = (patientRecord *)list->last->payload;

    if(strcmp(nodeRecord->recordId,firstRecord->recordId) < 0){
        insertFirst(list,node);
        return;
    }

    if(strcmp(nodeRecord->recordId ,lastRecord->recordId) > 0){
        insertLast(list,node);
        return;
    }

    listNode *tmp = list->first;
    patientRecord *tmpRecord;

    int i = 0;
    while(i < list->length){

        if(tmp->next != NULL){

            tmpRecord = (patientRecord *)tmp->next->payload;
            if(strcmp(nodeRecord->recordId,tmpRecord->recordId) < 0){
                
                node->next = tmp->next;
                tmp->next = node;
                list->length++;
                return;
            }
        }else{
            return;
        }

        tmp = tmp->next;
        i++;
    }

}

//Check If Patients Recrod Exists In List
patientRecord *findRecord(List *list, char *id){

    if((list == NULL) || (list->first == NULL)){
        return 0;
    }

    listNode *currNode = list->first;
    patientRecord *searchRecord = (patientRecord *)list->first->payload;

    while(currNode != NULL){
        searchRecord = (patientRecord *)currNode->payload;
        if(strcmp(id,searchRecord->recordId) == 0){
            return searchRecord;
        }
        currNode = currNode->next;
    }

    return NULL;
}

//Check If List Is Empty
int isEmpty(List *list){
    return list->length;
}

//Find Element In List
listNode* findElement(List *list, void *element){

    if(list == NULL){
        return NULL;
    }

    listNode *tmp = list->first;
    while(tmp != NULL){
        if(tmp->payload == element){
            return tmp;
        }
        tmp->next;
    }
}

//Find Records Of Patients Without Exit Date
int currentPatientsInList(List *list){

    if(list == NULL){
        return 0;
    }

    int currPatients = 0;

    listNode *currNode = list->first;
    patientRecord *pRecord;
    while(currNode != NULL){
        pRecord = (patientRecord *)currNode->payload;
        if(pRecord->exitDate[0] == '-'){
            currPatients++;
        }
        currNode = currNode->next;
    }

    return currPatients;
}

//Count patientRecords of given country
int countByCountry(List* list, char *country){

    if(list == NULL){
        return 0;
    }

    int recordsCount = 0;
    listNode *currNode = list->first;
    patientRecord *pRecord;

    while(currNode != NULL){
        pRecord = (patientRecord *)currNode->payload;
        if(strcmp(pRecord->Country,country) == 0){
            recordsCount++;
        }
        currNode = currNode->next;
    }

    return recordsCount;
}

//Count patientRecords of given disease
int countByDisease(List *list, char *disease){

    if(list == NULL){
        return 0;
    }

    int recordsCount = 0;
    listNode *currNode = list->first;
    patientRecord *pRecord;

    while(currNode != NULL){
        pRecord = (patientRecord *)currNode->payload;
        if(strcmp(pRecord->diseaseId,disease) == 0){
            recordsCount++;
        }
        currNode = currNode->next;
    }

    return recordsCount;
}

//Free A List Node
void freeListNode(listNode *node){
    free(node);
}

//Free A List
void freeList(List* list){
    if(list != NULL){

        listNode *tmp = list->first;
        listNode *rmv = tmp;;
        while(tmp != NULL){
            tmp = tmp->next;
            freeListNode(rmv);
            rmv = tmp;
        }
    }

    free(list);
}

//Free A List Node
void freePatientRecordsListNode(listNode *node){

    freePatientRecord(node->payload);
    free(node);
}

//Free A List
void freePatientRecordsList(List *list){

    if(list != NULL){

        listNode *tmp = list->first;
        listNode *rmv = tmp;;
        while(tmp != NULL){
            tmp = tmp->next;
            freePatientRecordsListNode(rmv);
            rmv = tmp;
        }
    }

    free(list);
}

//Print List
void printList(List *l){
    
    if(l == NULL){
        printf("List Not Constructed\n");
    }

    listNode *tmp = l->first;
    while(tmp != NULL){
        printPatientRecords((patientRecord *)tmp->payload);
        tmp = tmp->next;
    }
    printf("\n");
}