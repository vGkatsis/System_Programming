#include "../hdrs/error_handler.h"

//Check If Application Has Been Caled With Correct Number Of Arguments
int argCheck(int argc, char** argv, int *numOfWorkers, int *bufferSize, char **serverIP, int *serverPort,char **inputDir) {

	if(argc != 11) {

		printf("\nWrong Number Of Arguments Given.\n");
		printUsage();
		printf("error\n");
		return 0;
	}

	int opt;
	while((opt = getopt(argc,argv,"w:b:i:s:p:")) != -1) {

		switch(opt) {

			case 'i':
				(*inputDir) = (char *) malloc((strlen(optarg) + 1) * sizeof(char));
				strcpy((*inputDir),optarg);
				break;

			case 'w':

				*numOfWorkers = atoi(optarg);
				break;

			case 'b':

				*bufferSize = atoi(optarg);
				if((*bufferSize) < 1){
					printf("Buffer size should be at least 1 byte\n");
					return 0;
				}
				
				break;
		
			case 's':

				(*serverIP) = (char *) malloc((strlen(optarg) + 1) * sizeof(char));
				strcpy((*serverIP),optarg);
				break;

			case 'p':

				*serverPort = atoi(optarg);
				break;

			default:

				printf("Unknown character %c\n",(char)opt);
				break;
		}
	}

	return 1;
}

//Check if whoServer program has been called with correct arguments
int serverArgCheck(int argc, char **argv, int *queryPortNum, int *statisticsPortNum, int *numOfThreads, int *bufferSize){

	if(argc != 9) {

		printf("\nWrong Number Of Arguments Given.\n");
		printServerUsage();
		printf("error\n");
		return 0;
	}

	int opt;
	while((opt = getopt(argc,argv,"w:b:s:q:")) != -1) {

		switch(opt) {

			case 'w':

				*numOfThreads = atoi(optarg);
				break;

			case 'b':

				*bufferSize = atoi(optarg);
				if((*bufferSize) < 1){
					printf("Buffer size should be at least 1 \n");
					return 0;
				}
				
				break;
		
			case 's':

				*statisticsPortNum = atoi(optarg);
				break;

			case 'q':

				*queryPortNum = atoi(optarg);
				break;

			default:

				printf("Unknown character %c\n",(char)opt);
				break;
		}
	}

	return 1;
}

//Check if whoClient programm has been caled with correct arguments
int clientArgCheck(int argc, char **argv, FILE **queryFile, int *numThreads, int *serverPort, char **serverIP){

	if(argc != 9) {

		printf("\nWrong Number Of Arguments Given.\n");
		printClientUsage();
		printf("error\n");
		return 0;
	}

	int opt;
	int optionIndex = 0;

	static struct option long_options[] = {{"sp", required_argument, 0, 'p'},
										   {"sip", required_argument, 0, 'i'}};

	while((opt = getopt_long_only(argc,argv,"w:q:",long_options,&optionIndex)) != -1) {

		switch(opt) {

			case 'w':

				*numThreads = atoi(optarg);
				break;

			case 'q':

				if(((*queryFile) = fopen(optarg,"r")) == NULL) {
					printf("\nError while opening %s\n",optarg);
					return 0;
				}
				break;

			case 'p':

				*serverPort = atoi(optarg);
				break;

			case 'i':

				if(((*serverIP) = (char *) malloc((strlen(optarg) + 1) * sizeof(char))) == NULL){
					perror("Error while allocating space for serverIP\n");
					exit(1);
				}
				strcpy((*serverIP),optarg);
				break;

			default:

				printf("Unknown character %c\n",(char)opt);
				break;
		}
	}

	return 1;
}

//Print Helpping Message
void printUsage() {

    printf("\nProgram should be executed as follows:\n");
	printf("\t\t./master –w numWorkers -b bufferSize -s serverIP -p serverPort -i input_dir\n");
}

//Print Correct Usage Me
void printServerUsage(){

	printf("\nwhoServer program shoudl be executed as follows:\n");
	printf("\t\t./whoServer -q <queryPortNum> -s <ststisticsPortNum> -w <numThreads> -b <bufferSize>\n");
}

//Print Correct Usage Me
void printClientUsage(){

	printf("\nwhoClient program shoudl be executed as follows:\n");
	printf("\t\t./whoClient –q <queryFile> -w <numThreads> –sp <servPort> –sip <servIP>\n");
}

//Print Correct Usage Message
void printHelp() {

    printf("\nAfter reading of data is completed use on of the following commands:\n");
	printf("\n-/diseaseFrequency\n");
	printf("\n-/topk-AgeRanges\n");
	printf("\n-/searchPatientRecord\n");
	printf("\n-/numPatientAdmissions\n");
	printf("\n-/numPatientDischarges\n");
}