#include "../hdrs/utilityFunctions.h"

void listDirEntries(char *dirName, List **dirEntries){

    DIR *inputDirectory;                                                //Get content of input directory
    struct dirent *inputDirEntry; 

    char *entryName;
    listNode *countryNode;

    initializeList(dirEntries);                                         //Initialize the list which will contain the names of the directory's entries

    if((inputDirectory = opendir(dirName)) == NULL){                    //Exit if error occured while opening directory
        perror("Error while trying to open input directory. ");
        printf("%s\n",dirName);
    }

    while ((inputDirEntry = readdir(inputDirectory)) != NULL){                                      //While there are entries in the direcotry
        if((strcmp(inputDirEntry->d_name,".") != 0) && (strcmp(inputDirEntry->d_name,".."))){
            
            entryName = (char *)malloc((strlen(inputDirEntry->d_name) + 1) * sizeof(char));         //Save the entrie's name
            strcpy(entryName,inputDirEntry->d_name);
            
            char *countryDirPath;
            stringConcat(&countryDirPath,dirName,entryName);                                        //Create the path to the entriy

            if(inputDirEntry->d_type == DT_DIR){
                addSlashAtEnd(&countryDirPath);
                initializeListNode(&countryNode,countryDirPath);                                        //Insert entry path to the list
                insertLast((*dirEntries),countryNode);
            }
            else if(inputDirEntry->d_type == DT_REG){
                initializeListNode(&countryNode,countryDirPath);                                        //Insert entry path to the list
                insertOrderdDate((*dirEntries),countryNode);
            }else{
                perror("Not recognizable file type\n");
                exit(12);
            }

            free(entryName);
        }
    }

    closedir(inputDirectory);                                           //Close the directory
}