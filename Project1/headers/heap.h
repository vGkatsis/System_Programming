#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "linked_list.h"
#include "hashTable.h"

typedef struct HeapNode{

    struct HeapNode *left; 
    struct HeapNode *right; 
    struct HeapNode *next;
    struct HeapNode *parent;
    void *value;
}heapNode;

typedef struct Heap{

    heapNode *head;
    int numberOfNodes;
    int level;
}heap;

/*********************************
 *                               *
 *        Initialization         *
 *                               *
 *********************************/

void initializeHeapNode(heapNode **node, void *value);
void initializeHeap(heap **Heap);

/*********************************
 *                               *
 *           Utilities           *
 *                               *
 *********************************/

//Swap the values of two heap nodes
void swapHeapNodesValues(heapNode **nodeA, heapNode **nodeB);
//Compare to heap nodes based on there values
int compareNumberNodesValues(heapNode *nodeA, heapNode *nodeB);
//Insert a new node in the first empty leaf found from left to right
void insertHeapNode(heap *Heap, void *value);
//Get the root node of the heap
void *removeRootNode(heap *Heap);

/*********************************
 *                               *
 *        Getters/Setters        *
 *                               *
 *********************************/

heapNode *getLeft(heapNode *node);
heapNode *getRight(heapNode *node);
void *getValue(heapNode *node);
int getChildrenNum(heapNode *node);
heapNode *getHeapHead(heap *Heap);
int getHeapNumberOfNodes(heap *Heap);
int getLevel(heap *Heap);

void setLeft(heapNode *node, heapNode *newLeft);
void setRight(heapNode *node, heapNode *newRight);
void setValue(heapNode *node, void *newValue);
void setChildrenNum(heapNode *node, int newChildrenNum);
void setHeapHead(heap *Heap, heapNode *newHead);
void setHeapNumberOfNodes(heap *Heap, int newNumberOfNodes);
void setLevel(heap *Heap, int newLevel);

/*********************************
 *                               *
 *          Destruction          *
 *                               *
 *********************************/

void freeHeapNode(heapNode *node);
void freeHeap(heapNode *node);
#endif