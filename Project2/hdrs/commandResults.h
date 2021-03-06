#ifndef COMMAND_RESULTS_H
#define COMMAND_RESULTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hdrs/communication.h"
#include "../hdrs/utilityFunctions.h"
#include "../hdrs/logFileStatistics.h"

//Select the command for which results should be gathered
void commandResultsHandler(int serverFd, char *command, int *frequency, int curWorker ,int numOfWorkers, logStats *logStatistics);

//Gather results of fileReading function
void fileReadingResults(int serverFd);

//Gather results of listCountries command
void listCountriesResults(int serverFd);

//Gather results of diseaseFrequency command
void diseaseFrequencyResults(int serverFd, int *frequency, int curWorker ,int numOfWorkers);

//Gather results of topk-AgeRanges command
void topkAgeRangesResults(int serverFd);

//Gather results of searchPatientRecord command
void searchPatientRecordResults(int serverFd);

//Gather results of numPatientAdmissions command
void numPatientAdmissionsResults(int serverFd);

//Gather results of numPatientDischarges command
void numPatientDischargesResults(int serverFd);

//Gather log statistics from workers
void logStatisticsResults(int serverFd, int curWorker ,int numOfWorkers, logStats *logStatistics);

//Gathere intel when wrong command was given
void wrongCommand(int serverFd);

#endif