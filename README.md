# System_Programming

System Programming repo consists of three Projects all connected to each other.

### Project 1

Project 1 is the implementation of a **diseaseMonitor** programm which will keep track of patients with different diseases, and will be able to answer queries on these information. Since the programm must be able to handle big volumes of data it is implemented using space and time efficient data structures such as **Hash Tables** and **AVL Trees**.

![alt text](https://github.com/vGkatsis/System_Programming/blob/master/images/SysPro1.png?raw=true)

The diseaseMonitor is able to answer on the following queries:

- /**globalDiseaseStats &nbsp;\[date1 date2\]**  -> For each disease it prints the number of patients in the system. </br> 
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp;Arguments date1, date2 are optional.

- **/diseaseFrequency &nbsp;virusName &nbsp;date1 &nbsp;date2 &nbsp;\[country\]** -> Prints the number of patients recorded in the system </br>  &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp;&nbsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;between date1 and date2 with the given disease. Argument country is optional.

- **/insertPatientRecord &nbsp;Id &nbsp;FirstName &nbsp;LastName &nbsp;diseaseId &nbsp;country &nbsp;entryDate &nbsp;\[exitDate\]** -> Inserts new patient

- **recordPatientExit &nbsp;Id &nbsp;exitDate** -> Adds an exit date to a patients record with given Id

- **numCurrentPatients &nbsp;\[disease\]** -> Number of patients who have not ended treatment. Disease argument is optional

- **exit** -> Exits the programm

### Project 2

In Project 2 there are two tasks.

#### Task A

Create a system of communicating processes. </br> There is the diseaseAggregator and the worker processes. DiseaseAggregator gets queries from the user and passes them to the worker processes. Worker processes have each been given a part of the total input patients data, and each one executes the query on each own data. Then they return the results to the diseaseAggregator who gatheres them and returns the final answer to the user. All processes communicate using named pipes. 

![alt text](https://github.com/vGkatsis/System_Programming/blob/master/images/SysPro2.png?raw=true)

The diseaseAggregator answers to the following queries:

- **/listCountries** -> Prints each country allong with the pid of the worker process that handles data from this country.

- **/diseaseFrequency &nbsp;virusName &nbsp;date1 &nbsp;date2 &nbsp;\[country\]** -> Prints the number of patients recorded in the system </br>  &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp;&nbsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;between date1 and date2 with the given disease. Argument country is optional.

- **topk-AgeRanges &nbsp;k&nbsp;country&nbsp;disease&nbsp;date1&nbsp;date2** -> Prints top-k age ranges for given disease and country in given date range.

- **searchPatientRecord &nbsprecordId** -> Searches for a patient with given Id.

- **numPatientAdmissions &nbsp;disease &nbsp;date1 &nbsp;date2 &nbsp;\[country\]** -> Print number of admmited patients in </br>   &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp;&nbsp; given date range with given disease. Country argument is optional.

- **numPatientDischarges &nbsp;disease &nbsp;date1 &nbsp;date2 &nbsp;\[country\]** -> Print number of discharged patients in </br>   &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp;&nbsp; given date range with given disease. Country argument is optional.

- **exit** -> Exits the programm

#### Task B

Implement a bash script named create_infiles.sh which will create input data for the Task A. The script will take as input two files one with countries names and one with diseases names, the name of the input directry it will create, the number of files per directory and number of records per file.

### Project 3

Create another system of communicating processes. Programm whoClient will read queries from input files and will feed them in thread batches to the whoServer. WhoServer will accept incoming messages from whoClient threads and will pass them to master process. Master process much like the diseaseAggregator of Project2 passes the queries to worker processes. Worker processes get the results and connect with whoServer in order to return them. WhoServer then will return the result to whoCLient thread that gave the query and whoClient thread will print the results to the user. 

All interprocess communication is done via the network using IP addreses and ports, except for the communication of master and worker processes which happens via named pipes.

![alt text](https://github.com/vGkatsis/System_Programming/blob/master/images/SysPro3.png?raw=true)

Project 3 system of processes answers to the same queries as Project 2.

### Usage

- All projects contain an src directory which includes a makefile for compilation.
- Each executable will print a usage message in case it has been called incorectly.

