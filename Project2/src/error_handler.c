#include "../hdrs/error_handler.h"

//Check If Application Has Been Caled With Correct Number Of Arguments
int argCheck(int argc, char** argv, int *numOfWorkers, int *bufferSize, char **inputDir) {

	if(argc != 7) {

		printf("\nWrong Number Of Arguments Given.\n");
		printUsage();
		printf("error\n");
		return 0;
	}

	int opt;
	while((opt = getopt(argc,argv,"w:b:i:")) != -1) {

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
		}
	}

	return 1;
}

//Print Helpping Message
void printUsage() {

    printf("\nProgram should be executed as follows:\n");
	printf("\t\t./diseaseAggregator –w numWorkers -b bufferSize -i input_dir\n");
}

//Print Correct Usage Message
void printHelp() {

    printf("\nAfter reading of data is completed use on of the following commands:\n");
	printf("\n-/listCountries\n");
	printf("\n-/diseaseFrequency\n");
	printf("\n-/topk-AgeRanges\n");
	printf("\n-/searchPatientRecord\n");
	printf("\n-/numPatientAdmissions\n");
	printf("\n-/numPatientDischarges\n");
	printf("\n-/exit\n");
}