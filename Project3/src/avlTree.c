#include "../hdrs/avlTree.h"

treeNode *initializeTreeNode(date datekey,patientRecord* pRecord){

    treeNode *node = (treeNode *) malloc(1 * sizeof(treeNode));

    if(node == NULL){
        perror("\nError while allocating space for tree node\n");
        exit(4);
    }

    initializeList(&(node->valueList));
    
    listNode *newNode;
    
    initializeListNode(&newNode,pRecord);
    insertFirst(node->valueList,newNode);

    node->dateKey = datekey;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;

    return node;
}

void initializeAvlTree(avlTree **tree){

    if(((*tree) = (avlTree *) malloc(1 * sizeof(avlTree))) == NULL){
        perror("\nError while allocating space for avl tree\n");
        exit(4);
    }

    (*tree)->head = NULL;
    (*tree)->numberOfEntries = 0;
}

int maxHeight(int intA, int intB){
    
    if(intA >= intB){
        return intA;
    }else{
        return intB;
    }
}

int getBalanace(treeNode *node){

    if(node == NULL){
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

//Right rotation algorithm
treeNode *rightRotate(treeNode *node){

    treeNode *nodeX = node->left;
    treeNode *nodeT2 = nodeX->right;

    nodeX->right = node;
    node->left = nodeT2;

    node->height = maxHeight(getHeight(node->left),getHeight(node->right)) + 1;
    nodeX->height = maxHeight(getHeight(nodeX->left), getHeight(nodeX->right)) + 1;

    return nodeX;
}

//Left rotatation algorithm
treeNode *leftRotate(treeNode *node){

    treeNode *nodeY = node->right;
    treeNode *nodeT2 = nodeY->left;

    nodeY->left = node;
    node->right = nodeT2;

    node->height = maxHeight(getHeight(node->left), getHeight(node->right)) + 1;
    nodeY->height = maxHeight(getHeight(nodeY->left), getHeight(nodeY->right)) + 1;

    return nodeY;
}

//Insert a node into an avl-tree
treeNode *insertNode(treeNode *node, date dateKey, patientRecord *pRecord){

    //If en of tree is reached create leaf node
    if(node == NULL){
        return initializeTreeNode(dateKey,pRecord);
    }

    //Choose between left and right subtree in order to insert the new node
    if(compareDates(&dateKey,&(node->dateKey)) < 0){
        node->left = insertNode(node->left,dateKey,pRecord);
    }else if(compareDates(&dateKey,&(node->dateKey)) > 0){
        node->right = insertNode(node->right,dateKey,pRecord);
    }else{
        listNode *newNode;
    
        initializeListNode(&newNode,pRecord);
        insertOrderdString(node->valueList,newNode);
        return node;
    }

    //Set the height of the new node
    node->height = 1 + maxHeight(getHeight(node->left), getHeight(node->right)); 

    //Balance the new tree
    int balance = getBalanace(node);

    if((balance > 1) && (compareDates(&dateKey,&(node->left->dateKey)) < 0)){
        return rightRotate(node);
    }

    if((balance < -1) && (compareDates(&dateKey,&(node->right->dateKey)) > 0)){
        return leftRotate(node);
    }

    if((balance > 1) && (compareDates(&dateKey,&(node->left->dateKey)) > 0)){
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if((balance < -1) && (compareDates(&dateKey,&(node->right->dateKey)) < 0)){
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

//Count current patients in an avl-tree
int currentPatients(treeNode *tree){

    if(tree != NULL) 
    { 
        return currentPatientsInList(tree->valueList) + currentPatients(tree->left) + currentPatients(tree->right); 
    }else{
        return 0;
    }
    
}


//Return the number of entris in a given date range
int dateRangeEntries(treeNode *tree,date *date1,date *date2, char *fieldType, char *fieldValue){

    if(tree == NULL){
        return 0;
    }

    //If an entry is in the current date range
    if(((compareDates((&(tree->dateKey)),date1)) >= 0) && ((compareDates((&(tree->dateKey)),date2)) <= 0)){
        
        //Count entries containing a given country string
        if(strcmp(fieldType,"country") == 0){
            return countByCountry(tree->valueList,fieldValue) + dateRangeEntries(tree->left,date1,date2,fieldType,fieldValue) + dateRangeEntries(tree->right,date1,date2,fieldType,fieldValue);
        }
        //Count entries containing a given disease string
        if(strcmp(fieldType,"disease") == 0){
            return countByDisease(tree->valueList,fieldValue) + dateRangeEntries(tree->left,date1,date2,fieldType,fieldValue) + dateRangeEntries(tree->right,date1,date2,fieldType,fieldValue);
        }
        //Count entries entered or exited at a given date
        if(strcmp(fieldType,"date") == 0){
            return countByDate(tree->valueList,fieldValue) + dateRangeEntries(tree->left,date1,date2,fieldType,fieldValue) + dateRangeEntries(tree->right,date1,date2,fieldType,fieldValue);
        }
        //Count entries in a given age range
        if(strcmp(fieldType,"age+country") == 0){
            return countByAgeAndCountry(tree->valueList,fieldValue) + dateRangeEntries(tree->left,date1,date2,fieldType,fieldValue) + dateRangeEntries(tree->right,date1,date2,fieldType,fieldValue);
        }
        //Count exited patients from a given country
        if(strcmp(fieldType,"exited+country") == 0){
            return countExitedFromCountry(tree->valueList,fieldValue) + dateRangeEntries(tree->left,date1,date2,fieldType,fieldValue) + dateRangeEntries(tree->right,date1,date2,fieldType,fieldValue);
        }
        //Count exited patients with a given disease
        if(strcmp(fieldType,"exited+disease") == 0){
            return countExitedWithDisease(tree->valueList,fieldValue) + dateRangeEntries(tree->left,date1,date2,fieldType,fieldValue) + dateRangeEntries(tree->right,date1,date2,fieldType,fieldValue);
        }
        //Count entries with given disease in given age range
        if(strcmp(fieldType,"age+disease") == 0){
            return countByAgeAndDisease(tree->valueList,fieldValue) + dateRangeEntries(tree->left,date1,date2,fieldType,fieldValue) + dateRangeEntries(tree->right,date1,date2,fieldType,fieldValue);
        }
        //Count entries with given disease and country in given age range
        if(strcmp(fieldType,"age+disease+country") == 0){
            return countByAgeAndDiseaseAndCountry(tree->valueList,fieldValue) + dateRangeEntries(tree->left,date1,date2,fieldType,fieldValue) + dateRangeEntries(tree->right,date1,date2,fieldType,fieldValue);
        }
        //Count all enries in the date range
        if(strcmp(fieldType,"default") == 0){
            return tree->valueList->length + dateRangeEntries(tree->left,date1,date2,fieldType,fieldValue) + dateRangeEntries(tree->right,date1,date2,fieldType,fieldValue);
        }
     //If an entry is previous than date1 ignore the left subtree
    }else if((compareDates((&(tree->dateKey)),date1)) < 0){
        
        return dateRangeEntries(tree->right,date1,date2,fieldType,fieldValue);
     //If an entry is after date2 ignore right subtree
    }else{
        
        return dateRangeEntries(tree->left,date1,date2,fieldType,fieldValue);
    }
}

//Insert a given node to the avl-tree
void insertNodeToAVL(avlTree *tree, date dateKey, patientRecord *pRecord){

    tree->head = insertNode(tree->head,dateKey,pRecord);
    tree->numberOfEntries++;
}

treeNode * getTreeHead(avlTree *tree){
    
    if(tree == NULL){
//        perror("\nCant get treeHead of null tree\n");
        printf("error\n");
        return NULL;
    }

    return tree->head;
}

date getDateKey(treeNode *tree){
    
    if(tree == NULL){
//        perror("\nCant get key of null node\n");
        printf("error\n");
        exit(0);
    }
    return tree->dateKey;
}

int getHeight(treeNode *tree){
    if(tree == NULL){
        return 0;
    }
    return tree->height;
}

treeNode * getLeftNode(treeNode *tree){
     if(tree == NULL){
//        perror("\nCant get left node of null node\n");
        printf("error\n");
        return NULL;
    }
    return tree->left;
}

treeNode * getRightNode(treeNode *tree){
     
    if(tree == NULL){
//        perror("\nCant get right node of null node\n");
        printf("error\n");
        return NULL;
    }

    return tree->right;
}

int getNumberOfNodes(avlTree *tree){

    if(tree == NULL){
        return 0;
    }
    return tree->numberOfEntries;
}

void setHead(avlTree *tree, treeNode *newHead){
    tree->head = newHead;
}

void setDateKey(treeNode *tree, date newDateKey){
    tree->dateKey = newDateKey;
}

void setHeight(treeNode *tree, int newHeight){
    tree->height = newHeight;
}

void setLeftNode(treeNode *tree, treeNode *newLeftNode){
    tree->left = newLeftNode;
}

void setRightNode(treeNode *tree, treeNode *newRightNode){
    tree->right = newRightNode;
}

void setNumberOfNodes(avlTree *tree, int newNumberOfEntries){
    tree->numberOfEntries = newNumberOfEntries;
}

void freeTreeNode(treeNode *root){

    freeNonValueList(root->valueList);
    free(root);
}

void freeAvlTree(treeNode * root){

    if(root == NULL){
        return;
    }

    if(root != NULL) 
    { 
        freeAvlTree(root->left); 
        freeAvlTree(root->right); 
    } 
    freeTreeNode(root);
}