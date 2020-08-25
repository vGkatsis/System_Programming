CC = gcc
CFLAGS = -g -c
SRC = main.c date.c linked_list.c avlTree.c hashTable.c patientRecords.c error_handler.c commands.c stringHandler.c utilityFunctions.c communication.c fileReading.c  commandResults.c signalHandlers.c logFileStatistics.c
PROG = diseaseAggregator

WORKSRC = worker.c communication.c commands.c date.c hashTable.c avlTree.c patientRecords.c linked_list.c error_handler.c utilityFunctions.c stringHandler.c fileReading.c signalHandlers.c logFileStatistics.c commandResults.c
WORKPROG = worker

FIFO_CLIENT = ./named_pipes/client_named_pipe*
FIFO_SERVER = ./named_pipes/server_named_pipe*

LOG_FILES = ../logFiles/*

OBJS = $(SRC:.c=.o)
WORKOBJS = $(WORKSRC:.c=.o)

all: $(OBJS) $(WORKOBJS)
		$(CC) -g $(OBJS) -o $(PROG) -lm
		$(CC) -g $(WORKOBJS) -o $(WORKPROG) -lm

main.o: main.c
		$(CC) $(CFLAGS) main.c

date.o: date.c
		$(CC) $(CFLAGS) date.c

linked_list.o: linked_list.c
		$(CC) $(CFLAGS) linked_list.c

avlTree.o: avlTree.c
		$(CC) $(CFLAGS) avlTree.c

hashTable.o: hashTable.c
		$(CC) $(CFLAGS) hashTable.c

patientRecords.o: patientRecords.c
		$(CC) $(CFLAGS) patientRecords.c

error_handler.o: error_handler.c
		$(CC) $(CFLAGS) error_handler.c

commands.o: commands.c
		$(CC) $(CFLAGS) commands.c

worker.o: worker.c
		$(CC) $(CFLAGS) worker.c

stringHandler.o: stringHandler.c
		$(CC) $(CFLAGS) stringHandler.c

utilityFunctions.o: utilityFunctions.c
		$(CC) $(CFLAGS) utilityFunctions.c

communication.o: communication.c
		$(CC) $(CFLAGS) communication.c

fileReading.o: fileReading.c
		$(CC) $(CFLAGS) fileReading.c

commandResults.o: commandResults.c
		$(CC) $(CFLAGS) commandResults.c

signalHandlers.o: signalHandlers.c
		$(CC) $(CFLAGS) signalHandlers.c

logFileStatistics.o: logFileStatistics.c
		$(CC) $(CFLAGS) logFileStatistics.c

clean:
		rm -f $(OBJS) $(PROG) $(WORKOBJS) $(WORKPROG) $(FIFO_CLIENT) $(FIFO_SERVER) $(LOG_FILES)