#ifndef DATE_H
#define DATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Struct representing a date 
typedef struct Date{

    int year;
    int month;
    int day;
}date;

/****************************
 *                          *
 *      Initialization      *
 *                          *
 ****************************/

void initializeDate(date *Date);

/****************************
 *                          *
 *      Getters/Setters     *
 *                          *
 ****************************/

int getYear(date *Date);
int getMonth(date *Date);
int getDay(date *Date);

void setYear(date *Date, int newYear);
void setMonth(date *Date, int newMonth);
void setDay(date *Date, int newDay);

/******************************
 *                            *
 *          Utilities         *
 *                            *
 ******************************/

//Compares two dates
int compareDates(date *dateA, date *dateB);
//Parse a string date of format YYYY-MM-DD
int parseDate(char *stringDate, date *date);
//Print date and time in format YYYY-MM-DD
void printDate(date *Date);

#endif