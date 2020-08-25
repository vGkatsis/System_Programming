#ifndef LOG_FILE_STATISTICS_H
#define LOG_FILE_STATISTICS_H

#include <stdio.h>
#include "../hdrs/linked_list.h"

typedef struct LogStats{

    List *countriesList;
    int totalRequests;
    int successRequests;
    int failedRequests;
}logStats;

/*************************************************
 *                                               *
 *                  Initializations              *
 *                                               *
 *************************************************/

void initializeLogStats(logStats *stats);

/*************************************************
 *                                               *
 *              Getters - Setters                *
 *                                               *
 *************************************************/

List * getCountriesList(logStats *logStats);
int getTotalRequests(logStats *logStats);
int getSuccessRequests(logStats *logStats);
int getFailedRequests(logStats *logStats);

void setCountriesList(logStats *logStats, List *newList);
void setTotalRequests(logStats *logStats, int newTotalRequests);
void setSuccessRequests(logStats *logStats, int newSuccessRequests);
void setFailedRequests(logStats *logStats, int newFailedRequests);

/*************************************************
 *                                               *
 *                   Utilities                   *
 *                                               *
 *************************************************/

void incrementTotalRequests(logStats *logStats);
void incrementSuccessRequests(logStats *logStats);
void incrementFailedRequests(logStats *logStats);

#endif