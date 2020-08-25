#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wait.h>

extern int newWorkerData;
extern int childInteruption;
extern int workerInteruptionSignal;
extern int aggregatorInteruptionSignal;

//Signal handler handling SIGINT and SIGQUIT for worker(server)
void interuptWorker(int sigNum);

//Signal handler handling SIGINT and SIGQUIT for aggregator(client)
void interuptAggregator(int sigNum);

//Signal handler handling SIGUSR1 for worker(server)
void newDataForWorker(int sigNum);

//Signal handler handling SIGCHILD for aggregator(client)
void handleChildInteruption(int sigNum);

#endif