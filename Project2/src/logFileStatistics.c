#include "../hdrs/logFileStatistics.h"

void initializeLogStats(logStats *stats){

    stats->countriesList = NULL;
    stats->totalRequests = 0;
    stats->successRequests = 0;
    stats->failedRequests = 0;
}

List * getCountriesList(logStats *logStats){
    return logStats->countriesList;
}

int getTotalRequests(logStats *logStats){
    return logStats->totalRequests;
}

int getSuccessRequests(logStats *logStats){
    return logStats->successRequests;
}

int getFailedRequests(logStats *logStats){
    return logStats->failedRequests;
}

void setCountriesList(logStats *logStats, List *newList){
    logStats->countriesList = newList;
}

void setTotalRequests(logStats *logStats, int newTotalRequests){
    logStats->totalRequests = newTotalRequests;
}

void setSuccessRequests(logStats *logStats, int newSuccessRequests){
    logStats->successRequests = newSuccessRequests;
}

void setFailedRequests(logStats *logStats, int newFailedRequests){
    logStats->failedRequests = newFailedRequests;
}


void incrementTotalRequests(logStats *logStats){
    logStats->totalRequests++;
}

void incrementSuccessRequests(logStats *logStats){
    logStats->successRequests++;
}

void incrementFailedRequests(logStats *logStats){
    logStats->failedRequests++;
}
