CC = gcc
CFLAGS = -g -c
SRC = main.c date.c linked_list.c avlTree.c hashTable.c patientRecords.c error_handler.c commands.c stringHandler.c utilityFunctions.c communication.c fileReading.c  commandResults.c signalHandlers.c
PROG = master

WORKSRC = worker.c communication.c commands.c date.c hashTable.c avlTree.c patientRecords.c linked_list.c error_handler.c utilityFunctions.c stringHandler.c fileReading.c signalHandlers.c commandResults.c
WORKPROG = worker

SERVERSRC = whoServer.c error_handler.c circularBuffer.c fdType.c whoServerFunctions.c communication.c commandResults.c stringHandler.c linked_list.c date.c hashTable.c avlTree.c patientRecords.c commands.c utilityFunctions.c fileReading.c
SERVERPROG = whoServer

CLIENTSRC = whoClient.c error_handler.c whoClientFunctions.c
CLIENTPROG = whoClient

FIFO_CLIENT = ./named_pipes/client_named_pipe*
FIFO_SERVER = ./named_pipes/server_named_pipe*

OBJS = $(SRC:.c=.o)
WORKOBJS = $(WORKSRC:.c=.o)
SERVEROBJS = $(SERVERSRC:.c=.o)
CLIENTOBJS = $(CLIENTSRC:.c=.o)

all: $(OBJS) $(WORKOBJS) $(SERVEROBJS) $(CLIENTOBJS)
		$(CC) -g $(OBJS) -o $(PROG) -lm
		$(CC) -g $(WORKOBJS) -o $(WORKPROG) -lm
		$(CC) -g $(SERVEROBJS) -o $(SERVERPROG) -lm -lpthread
		$(CC) -g $(CLIENTOBJS) -o $(CLIENTPROG) -lm -lpthread

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

whoServer.o: whoServer.c
		$(CC) $(CFLAGS) whoServer.c

whoClient.o: whoClient.c
		$(CC) $(CFLAGS) whoClient.c

whoClientFunctions.o: whoClientFunctions.c
		$(CC) $(CFLAGS) whoClientFunctions.c

circularBuffer.o: circularBuffer.c
		$(CC) $(CFLAGS) circularBuffer.c

fdType.o: fdType.c
		$(CC) $(CFLAGS) fdType.c

whoServerFunctions.o: whoServerFunctions.c
		$(CC) $(CFLAGS) whoServerFunctions.c

clean:
		rm -f $(OBJS) $(PROG) $(WORKOBJS) $(WORKPROG) $(SERVEROBJS) $(SERVERPROG) $(CLIENTOBJS) $(CLIENTPROG) $(FIFO_CLIENT) $(FIFO_SERVER)