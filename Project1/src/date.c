#include "../headers/date.h"


void initializeDate(date *Date){

    Date->year = 0;
    Date->month = 0;
    Date->day = 0;
}

int getYear(date *Date){
    return Date->year;
}

int getMonth(date *Date){
    return Date->month;
}

int getDay(date *Date){
    return Date->day;
}

void setYear(date *Date, int newYear){
    
    if(newYear > 1900){
        Date->year = newYear;
    }
}

void setMonth(date *Date, int newMonth){
    
    if((newMonth > 1) && (newMonth < 12)){
        Date->month = newMonth;
    }
}

void setDay(date *Date, int newDay){
    
    if((newDay > 1) && (newDay < 31)){
        Date->day = newDay;
    }
}

//Compares two dates
int compareDates(date *dateA, date *dateB){

    //If dateA is previous than dateB return -1
    if(dateA->year < dateB->year){
        return -1;
      //If dateB is previous than dateA return 1
    } else if(dateA->year > dateB->year){
        return 1;
    } else{
        if(dateA->month < dateB->month){
            return -1;
        } else if(dateA->month > dateB->month){
            return 1;
        } else{
            if(dateA->day < dateB->day){
                return -1;
            } else if(dateA->day > dateB->day){
                return 1;
            } else{
                //If both dates are same return 0
                return 0;
            }
        }
    }
}

//Parse a string date of format DD-MM-YYYY
int parseDate(char *stringDate, date *date){

    char *tmpStringDate,*token, *trash;
    char delim[2] = "-";

    tmpStringDate = (char *)malloc((strlen(stringDate) + 1) * sizeof(char));
    strcpy(tmpStringDate,stringDate);

    token = strtok(tmpStringDate,delim);
    date->day = strtol(token,&trash,10);

    token = strtok(NULL,delim);
    date->month = strtol(token,&trash,10);
    
    token = strtok(NULL,delim);
    date->year = strtol(token,&trash,10);

    free(tmpStringDate);
    return 1;
}

//Print date and time in format DD-MM-YYYY
void printDate(date *Date){

    if(Date->day < 10){
        printf("0%d-",Date->day);
    } else{
        printf("%d-",Date->day);
    }

    if(Date->month < 10){
        printf("0%d-",Date->month);
    } else{
        printf("%d-",Date->month);
    }

    printf("%d   ",Date->year);
}