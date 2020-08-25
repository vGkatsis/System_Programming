#include <stdio.h>
#include <stdlib.h>
#include "../hdrs/linked_list.h"
#include "../hdrs/patientRecords.h"

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

//Remove the FrrstNode from the list
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

//Insert filePaths In Alphabetical order(based on date part)
void insertOrderdDate(List *list, listNode *node){

    if((list == NULL) || (list->first == NULL)){
        insertFirst(list,node);
        return;
    }

    char *nodeFilePath, *firstFilePath, *lastFilePath, *tmpFilePath;
    char *charNodeDate, *charFirstDate, *charLastDate, *tmpNodeDate;
    date nodeDate, firstDate, lastDate, tmpDate;

    if((nodeFilePath = (char *) malloc((strlen((char *)node->payload) + 1) * sizeof(char))) == NULL){
        perror("Error while allocating space for nodeFilePath\n");
        exit(1);
    }

    if((firstFilePath = (char *) malloc((strlen((char *)list->first->payload) + 1) * sizeof(char))) == NULL){
        perror("Error while allocating space for firstFilePath\n");
        exit(1);
    }

    if((lastFilePath = (char *) malloc((strlen((char *)list->last->payload) + 1) * sizeof(char))) == NULL){
        perror("Error while allocating space for lastFilePath\n");
        exit(1);
    }

    strcpy(nodeFilePath,(char *)node->payload);
    strcpy(firstFilePath,(char *)list->first->payload);
    strcpy(lastFilePath,(char *)list->last->payload);

    strtok(nodeFilePath,".");
    strtok(nodeFilePath,"/");
    strtok(NULL,"/");
    strtok(NULL,"/");
    charNodeDate = strtok(NULL,"/");

    strtok(firstFilePath,".");
    strtok(firstFilePath,"/");
    strtok(NULL,"/");
    strtok(NULL,"/");
    charFirstDate = strtok(NULL,"/");

    strtok(lastFilePath,".");
    strtok(lastFilePath,"/");
    strtok(NULL,"/");
    strtok(NULL,"/");
    charLastDate = strtok(NULL,"/");

    parseDate(charNodeDate,&nodeDate);
    parseDate(charFirstDate,&firstDate);
    parseDate(charLastDate,&lastDate);

    if(compareDates(&nodeDate,&firstDate) < 0){
        insertFirst(list,node);
        free(nodeFilePath);
        free(firstFilePath);
        free(lastFilePath);
        return;
    }

    if(compareDates(&nodeDate,&lastDate) > 0){
        insertLast(list,node);
        free(nodeFilePath);
        free(firstFilePath);
        free(lastFilePath);
        return;
    }

    free(firstFilePath);
    free(lastFilePath);

    listNode *tmp = list->first;

    int i = 0;
    while(i < list->length){

        if(tmp->next != NULL){

            if((tmpFilePath = (char *) malloc((strlen((char *)tmp->next->payload) + 1) * sizeof(char))) == NULL){
                perror("Error while allocating space for tmpFilePath\n");
                exit(1);
            }

            strcpy(tmpFilePath,(char *)tmp->next->payload);

            strtok(tmpFilePath,".");
            strtok(tmpFilePath,"/");
            strtok(NULL,"/");
            strtok(NULL,"/");
            tmpNodeDate = strtok(NULL,"/");

            parseDate(tmpNodeDate,&tmpDate);

            if(compareDates(&nodeDate,&tmpDate) < 0){
                
                node->next = tmp->next;
                tmp->next = node;
                list->length++;
                
                free(nodeFilePath);
                free(tmpFilePath);
                return;
            }

            free(tmpFilePath);
        }else{
            free(nodeFilePath);
            free(tmpFilePath);
            return;
        }

        tmp = tmp->next;
        i++;
    }

    free(nodeFilePath);
    free(tmpFilePath);
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

//String exists in list
listNode* stringExistsInList(List *list, char *element){

    if(list == NULL){
        return NULL;
    }

    listNode *tmp = list->first;
    while(tmp != NULL){
        if(strcmp(element,(char *)tmp->payload) == 0){
            return tmp;
        }
        tmp = tmp->next;
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
        tmp = tmp->next;
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

//Count patientRecords entered or exited at a given date
int countByDate(List *list, char *date){

    if(list == NULL){
        return 0;
    }

    int recordsCount = 0;
    listNode *currNode = list->first;
    patientRecord *pRecord;
    struct Date givenDate, enterDate, exitDate;

    parseDate(date,&givenDate);
    while(currNode != NULL){
        pRecord = (patientRecord *)currNode->payload;

        parseDate(getEntryDate(pRecord),&enterDate);
        int exitExists = parseDate(getExitDate(pRecord),&exitDate);
        
        if((compareDates(&givenDate,&enterDate) == 0) || ((exitExists) && (compareDates(&givenDate,&exitDate) == 0))){
            recordsCount++;
        }
        currNode = currNode->next;
    }

    return recordsCount;
}

//Count patientRecords of a given age range
int countByAgeAndCountry(List *list, char *ageCountry){

    if(list == NULL){
        return 0;
    }

    int recordsCount = 0;
    int minAge, maxAge;
    char *tmpAgeCountry, *country;
    listNode *currNode = list->first;
    patientRecord *pRecord;

    if((tmpAgeCountry = (char *) malloc((strlen(ageCountry) + 1) * sizeof(char))) == NULL){
        perror("Error while allocating space for tmpDiseaseAge string\n");
        exit(1);
    }
    strcpy(tmpAgeCountry,ageCountry);

    country = strtok(ageCountry,"+");
    minAge = atoi(strtok(NULL,"+"));
    maxAge = atoi(strtok(NULL,""));

    while(currNode != NULL){
        pRecord = (patientRecord *)currNode->payload;
        if((strcmp(getCountry(pRecord),country) == 0) && (getAge(pRecord) >= minAge) && (getAge(pRecord) <= maxAge)){
            recordsCount++;
        }
        currNode = currNode->next;
    }

    free(tmpAgeCountry);

    return recordsCount;
}

//Count exited patientRecords from given country
int countExitedFromCountry(List *list, char *country){

    if(list == NULL){
        return 0;
    }

    int recordsCount = 0;
    listNode *currNode = list->first;
    patientRecord *pRecord;

    while(currNode != NULL){
        pRecord = (patientRecord *)currNode->payload;
        if((getExitDate(pRecord)[0] != '-') && (strcmp(getCountry(pRecord),country) == 0)){
            recordsCount++;
        }
        currNode = currNode->next;
    }

    return recordsCount;
}

//Count exited patientRecords with given disease
int countExitedWithDisease(List *list, char *disease){

    if(list == NULL){
        return 0;
    }

    int recordsCount = 0;
    listNode *currNode = list->first;
    patientRecord *pRecord;

    while(currNode != NULL){
        pRecord = (patientRecord *)currNode->payload;
        if((getExitDate(pRecord)[0] != '-') && (strcmp(getDiseaseID(pRecord),disease) == 0)){
            recordsCount++;
        }
        currNode = currNode->next;
    }

    return recordsCount;
}

//Count exited patientRecords with given disease in given age range
int countByAgeAndDisease(List *list, char *diseaseAndAge){

    if(list == NULL){
        return 0;
    }

    char *disease, *tmpDiseaseAge;
    int minAge,maxAge;

    if((tmpDiseaseAge = (char *) malloc((strlen(diseaseAndAge) + 1) * sizeof(char))) == NULL){
        perror("Error while allocating space for tmpDiseaseAge string\n");
        exit(1);
    }
    strcpy(tmpDiseaseAge,diseaseAndAge);

    disease = strtok(tmpDiseaseAge,"+");
    minAge = atoi(strtok(NULL,"+"));
    maxAge = atoi(strtok(NULL,""));

    int recordsCount = 0;
    listNode *currNode = list->first;
    patientRecord *pRecord;

    while(currNode != NULL){
        pRecord = (patientRecord *)currNode->payload;
        if((strcmp(getDiseaseID(pRecord),disease) == 0) &&
                                    (getAge(pRecord) >= minAge) &&
                                        (getAge(pRecord) <= maxAge)){
            recordsCount++;
        }
        currNode = currNode->next;
    }

    free(tmpDiseaseAge);

    return recordsCount;
}

//Count entries with given disease and country in given age range
int countByAgeAndDiseaseAndCountry(List *list, char *diseaseAndCountryAndAge){

    if(list == NULL){
        return 0;
    }

    char *disease, *tmpDiseaseAge, *country;
    int minAge,maxAge;

    if((tmpDiseaseAge = (char *) malloc((strlen(diseaseAndCountryAndAge) + 1) * sizeof(char))) == NULL){
        perror("Error while allocating space for tmpDiseaseAge string\n");
        exit(1);
    }
    strcpy(tmpDiseaseAge,diseaseAndCountryAndAge);

    disease = strtok(tmpDiseaseAge,"+");
    country = strtok(NULL,"+");
    minAge = atoi(strtok(NULL,"+"));
    maxAge = atoi(strtok(NULL,""));

    int recordsCount = 0;
    listNode *currNode = list->first;
    patientRecord *pRecord;

    while(currNode != NULL){
        pRecord = (patientRecord *)currNode->payload;
        if((strcmp(getDiseaseID(pRecord),disease) == 0) &&
                        (strcmp(getCountry(pRecord),country) == 0) &&
                                (getAge(pRecord) >= minAge) &&
                                        (getAge(pRecord) <= maxAge)){
            recordsCount++;
        }
        currNode = currNode->next;
    }

    free(tmpDiseaseAge);

    return recordsCount;
}

//Free A List Node
void freeListNode(listNode *node){

    if(node == NULL){
        return;
    }

    free(node->payload);
    free(node);
}

//Free A List
void freeList(List* list){
    if(list != NULL){

        listNode *tmp = list->first;
        listNode *rmv = tmp;
        while(tmp != NULL){
            tmp = tmp->next;
            freeListNode(rmv);
            rmv = tmp;
        }
    }

    free(list);
}

//Free A List Node But Dont free Values
void freeNonValueListNode(listNode *node){

    if(node == NULL){
        return;
    }

    free(node);
}

//Free A List But Dont free Values
void freeNonValueList(List* list){
    if(list != NULL){

        listNode *tmp = list->first;
        listNode *rmv = tmp;
        while(tmp != NULL){
            tmp = tmp->next;
            freeNonValueListNode(rmv);
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