#include "../headers/error_handler.h"

//Check If Application Has Been Caled With Correct Number Of Arguments
int argCheck(int argc, char** argv, int *diseaseHashNum, int *countryHashNum, int *bucket_size, FILE **patientRecordsFile) {

	if(argc != 9) {

		printf("\nWrong Number Of Arguments Given.\n");
		printUsage();
		printf("error\n");
		return 0;
	}

	int opt;
	int option_index = 0;
	char *records;

	static struct option long_options[] = {{"h1", required_argument, 0, 'd'},
										   {"h2", required_argument, 0, 'c'}};

	while((opt = getopt_long_only(argc,argv,"p:b:",long_options,&option_index)) != -1) {

		switch(opt) {

			case 'p':
				records = (char *) malloc((strlen(optarg) + 1) * sizeof(char));
				strcpy(records,optarg);
				if((*patientRecordsFile = fopen(records,"r")) == NULL) {
					printf("\nError while opening %s\n",records);
                    free(records);
					return 0;
				}

				free(records);
				break;

			case 'd':
				
				*diseaseHashNum = atoi(optarg);
				break;
			
			case 'c':

				*countryHashNum = atoi(optarg);
				break;

			case 'b':

				*bucket_size = atoi(optarg);
				if((*bucket_size) < sizeof(bucketEntry)){
					printf("Bucket size should be at least %ld bytes which is the size of 1 bucket entry\n",sizeof(bucketEntry));
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
	printf("\t\t./diseaseMonitor -p <patientRecordsFile> -h1 <diseaseHashTableNumOfEntries> -h2 <countryHashTableNumOfEntries> -b <bucketSize>\n");
}

//Print Correct Usage Message
void printHelp() {

    printf("\nAfter reading of data is completed use on of the following commands:\n");
	printf("\n-globalDiseaseStats\n");
	printf("\n-diseaseFrequency\n");
	printf("\n-topk-Diseases\n");
	printf("\n-topk-Countries\n");
	printf("\n-insertPatientRecord\n");
	printf("\n-recordPatientExit\n");
	printf("\n-numCurrentPatients\n");
	printf("\n-exit\n");
}