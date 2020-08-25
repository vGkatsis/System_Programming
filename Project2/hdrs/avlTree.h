/******************************************************************
 *                                                                *
 *                  Code taken and modified from                  *
 *      https://www.geeksforgeeks.org/avl-tree-set-1-insertion/   *
 *                                                                *
 ******************************************************************/

#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include "date.h"
#include "linked_list.h"
#include "patientRecords.h"

typedef struct TreeNode{

    date dateKey;
    List *valueList;
    struct TreeNode *left; 
    struct TreeNode *right; 
    int height; 
}treeNode;

typedef struct AVLTree{

    treeNode *head;
    int numberOfEntries;
}avlTree;

/*********************************
 *                               *
 *        Initialization         *
 *                               *
 *********************************/

treeNode *initializeTreeNode(date dateKey,patientRecord *pRecord);
void initializeAvlTree(avlTree **tree);

/*********************************
 *                               *
 *           Utilities           *
 *                               *
 *********************************/

int maxHeight(int intA, int intB);
int getBalanace(treeNode *node);
int currentPatients(treeNode *tree);
int dateRangeEntries(treeNode *tree,date *date1,date *date2, char *fieldType, char *fieldValue);
treeNode *rightRotate(treeNode *node);
treeNode *leftRotate(treeNode *node);
treeNode *insertNode(treeNode *node, date dateKey, patientRecord *pRecord);
void insertNodeToAVL(avlTree *tree, date dateKey,patientRecord *pRecord);
 

/*********************************
 *                               *
 *        Getters/Setters        *
 *                               *
 *********************************/

treeNode * getTreeHead(avlTree *tree);
date getDateKey(treeNode *tree);
int getHeight(treeNode *tree);
int getNumberOfEntries(avlTree *tree);
treeNode * getLeftNode(treeNode *tree);
treeNode * getRightNode(treeNode *tree);

void setHead(avlTree *tree, treeNode *newHead);
void setDateKey(treeNode *tree, date newDateKey);
void setHeight(treeNode *tree, int newHeight);
void setLeftNode(treeNode *tree, treeNode *newLeftNode);
void setRightNode(treeNode *tree, treeNode *newRightNode);
void setNumberOfEntries(avlTree *tree, int newNumberOfEntries);

/*********************************
 *                               *
 *          Destruction          *
 *                               *
 *********************************/

void freeTreeNode(treeNode *node);
void freeAvlTree(treeNode * root);
#endif