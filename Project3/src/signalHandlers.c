#include "../hdrs/signalHandlers.h"

int newWorkerData = 0;
int childInteruption = 0;
int workerInteruptionSignal = 0;
int aggregatorInteruptionSignal = 0;

void interuptWorker(int sigNum){

    workerInteruptionSignal = 1;
}

void interuptAggregator(int sigNum){

    aggregatorInteruptionSignal = 1;
}

void newDataForWorker(int sigNum){

    newWorkerData = 1;
}

void handleChildInteruption(int sigNum){

    childInteruption = 1;
}