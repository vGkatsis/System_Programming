CC = gcc
CFLAGS = -g -c
SRC = main.c linked_list.c error_handler.c commands.c fileReading.c patientRecords.c date.c avlTree.c hashTable.c heap.c
HDR = linked_list.h error_handler.h commands.h fileReading.h patientRecords.h date.h avlTree.h hashTable.h heap.h
PROG = diseaseMonitor

OBJS = $(SRC:.c=.o)

all: $(OBJS)
				$(CC) -g $(OBJS) -o $(PROG) -lm

main.o: main.c
				$(CC) $(CFLAGS) main.c

linked_list.o: linked_list.c
				$(CC) $(CFLAGS) linked_list.c

error_handler.o: error_handler.c
				$(CC) $(CFLAGS) error_handler.c

commands.o: commands.c
				$(CC) $(CFLAGS) commands.c

fileReading.o: fileReading.c
				$(CC) $(CFLAGS) fileReading.c

patientRecords.o: patientRecords.c
				$(CC) $(CFLAGS) patientRecords.c

date.o:date.c
				$(CC) $(CFLAGS) date.c

avlTree.o: avlTree.c
				$(CC) $(CFLAGS) avlTree.c

hashTable.o: hashTable.c
				$(CC) $(CFLAGS) hashTable.c

heap.o: heap.c
				$(CC) $(CFLAGS) heap.c

clean:
				rm -f $(OBJS) $(PROG)