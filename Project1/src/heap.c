#include "../headers/heap.h"

void initializeHeapNode(heapNode **node, void *value){

    if(((*node) = (heapNode *)malloc(1 * sizeof(heapNode))) == NULL){
        perror("Error while allocating space for heapNode\n");
        exit(8);
    }

    (*node)->left = NULL;
    (*node)->right = NULL;
    (*node)->next = NULL;
    (*node)->parent = NULL;
    (*node)->value = value;
}

void initializeHeap(heap **Heap){

    if(((*Heap) = (heap *)malloc(1 * sizeof(heap))) == NULL){
        perror("Error while allocating space for Heap\n");
        exit(8);
    }

    (*Heap)->head = NULL;
    (*Heap)->level = 0;
    (*Heap)->numberOfNodes = 0;
}


//Swap the values of two heap nodes
void swapHeapNodesValues(heapNode **nodeA, heapNode **nodeB){

    void *tmpValue = (*nodeA)->value;

    (*nodeA)->value = (*nodeB)->value;
    (*nodeB)->value = tmpValue;
}

//Compare to heap nodes based on there values
int compareNumberNodesValues(heapNode *nodeA, heapNode *nodeB){

    bucketEntry *entryA = (bucketEntry *)nodeA->value;
    bucketEntry *entryB = (bucketEntry *)nodeB->value;

    int *numA = (int *)(entryA->value);
    int *numB = (int *)(entryB->value);

    if((*numA) < (*numB)){
        return -1;
    }else if((*numA) > (*numB)){
        return 1;
    }else{
        return 0;
    }

}

//Insert a new node in the first empty leaf position
void insertHeapNode(heap *Heap, void *value){
    
    if(Heap == NULL){
        return;
    }
    
    heapNode *tmpNode, *currNode, *heightNode;
    
    initializeHeapNode(&tmpNode,value);
    if(Heap->head == NULL){
        Heap->head = tmpNode;
        Heap->level++;
        Heap->numberOfNodes++;
    
        return;
    }

    currNode = Heap->head;
    //Reach the start of level before last level
    for(int i = 1; i < Heap->level; i++){
        currNode = currNode->left;
    }
    heightNode = currNode;

    //Find the last node in the level before last level
    while((currNode->left != NULL) && (currNode->right != NULL)){
        currNode = currNode->next;
    }

    //Connect new node with parent node
    tmpNode->parent = currNode;
    if(currNode->left == NULL){
        currNode->left = tmpNode;
    }else{
        currNode->right = tmpNode;
    }
    Heap->numberOfNodes++;
    if(Heap->numberOfNodes == (pow(2,(Heap->level + 1)) - 1)){
        Heap->level++;
    }

    //Find start of last level and connect with the rest of the nodes
    currNode = heightNode->left;

    //Find the last node in the last level
    while(currNode->next != NULL){
        currNode = currNode->next;
    }
    if(currNode != tmpNode){
        currNode->next = tmpNode;
    }

    heapNode *parentNode = tmpNode->parent;
    //Retain the max-heap property
    while((tmpNode != NULL) && (parentNode != NULL) && (compareNumberNodesValues(tmpNode,parentNode) > 0)){

        swapHeapNodesValues(&tmpNode,&parentNode);

        tmpNode = parentNode;
        parentNode = parentNode->parent;
    }
}

//Get the root node of the heap
void *removeRootNode(heap *Heap){

    if((Heap == NULL) || (Heap->head == NULL)){
        perror("Error while trying to access uninitialized heap.\n");
        return NULL;
    }

    void *returnValue = Heap->head->value;
    
    if(Heap->numberOfNodes == 1){
        free(Heap->head);
        Heap->head = NULL;
        Heap->level--;
        Heap->numberOfNodes--;
    
        return returnValue;
    }

    heapNode *lastNode, *heightNode;
    heapNode *currNode = Heap->head;

    heightNode = Heap->head;
    //If the heap is a full tree
    if(Heap->numberOfNodes == (pow(2,(Heap->level)) - 1)){
        while(currNode->right->right != NULL){
            currNode = currNode->right;
        }
        lastNode = currNode->right;
        lastNode->parent = NULL;
        currNode->right = NULL;
    
        for(int i = 1; i < Heap->level; i++){
            heightNode = heightNode->left;
        }
    }else{
        //Search for the father of the last node
        for(int i = 1; i < Heap->level - 1; i++){
            currNode = currNode->left;
        }
        heightNode = currNode->left;

        //Find the last node
        while((currNode->left != NULL) && (currNode->right != NULL)){
            if((currNode->next->left == NULL) && (currNode->next->right == NULL)){
                break;
            }else{
                currNode = currNode->next;
            }
        }
        
        if(currNode->left == NULL){
            lastNode = currNode->right;
            currNode->right = NULL;
        }else{
            lastNode = currNode->left;
            currNode->left = NULL;
        }
    }

    //En error has occured
    if(lastNode == NULL){
        return NULL;
    }

    //Value of last node becomes value of head node
    Heap->head->value = lastNode->value;

    //Remove from last level
    if(heightNode != lastNode){
        
        while((heightNode->next != NULL) && (heightNode->next != lastNode)){
                    heightNode = heightNode->next;
        }

        if((heightNode->next != NULL) && (heightNode->next == lastNode)){
            free(heightNode->next);
            heightNode->next = NULL;
        }
    }else{

        Heap->level--;
        free(heightNode);
    }
    Heap->numberOfNodes--;

    //Deletion heapify
    heapNode *swapNode;

    currNode = Heap->head;
    int noMoreComparisons;
    while((currNode != NULL) && ((currNode->left != NULL) || (currNode->right != NULL))){
        noMoreComparisons = 0;

        if((currNode->left != NULL) && (compareNumberNodesValues(currNode,currNode->left) < 0)){
            
            swapNode = currNode->left;
            noMoreComparisons++;
        }
        
        if((currNode->right != NULL) && (compareNumberNodesValues(currNode,currNode->right) < 0)){
                
            swapNode = currNode->right;
            noMoreComparisons++;
        }
        
        if(!noMoreComparisons){
            break;
        }
    
        swapHeapNodesValues(&currNode,&swapNode);
        currNode = swapNode;
    }
    


    return returnValue;
}


heapNode *getLeft(heapNode *node){
    if(node == NULL){
        return NULL;
    }
    return node->left;
}

heapNode *getRight(heapNode *node){
    
    if(node == NULL){
        return NULL;
    }
    return node->right;
}

void *getValue(heapNode *node){

    if(node == NULL){
        return NULL;
    }
    return node->value;
}

heapNode *getHeapHead(heap *Heap){

    if(Heap == NULL){
        return NULL;
    }
    return Heap->head;
}

int getHeapNumberOfNodes(heap *Heap){

    if(Heap == NULL){
        return -1;
    }
    return Heap->numberOfNodes;
}

int getLevel(heap *Heap){

    if(Heap == NULL){
        return -1;
    }
    return Heap->level;
}

void setLeft(heapNode *node, heapNode *newLeft){
    node->left = newLeft;
}

void setRight(heapNode *node, heapNode *newRight){
    node->right = newRight;
}

void setValue(heapNode *node, void *newValue){
    node->value = newValue;
}

void setHeapHead(heap *Heap, heapNode *newHead){
    Heap->head = newHead;
}

void setHeapNumberOfNodes(heap *Heap, int newNumberOfNodes){
    Heap->numberOfNodes = newNumberOfNodes;
}

void setLevel(heap *Heap, int newLevel){
    Heap->level = newLevel;
}

void freeHeapNode(heapNode *node){

    if(node == NULL){
        return;
    }
    free(node);
}

void freeHeap(heapNode *node){

    if(node == NULL){
        return;
    }

    if(node != NULL) 
    { 
        freeHeap(node->left); 
        freeHeap(node->right); 
    } 
    freeHeapNode(node);
    
}