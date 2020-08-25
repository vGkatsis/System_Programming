#!/bin/bash

#Usage message explaining correct script call
USAGE="Usage: create_infiles.sh 'diseasesFile' 'countriesFile' 'input_dir' 'numFilesPerDirectory' 'numRecordsPerFile'"

#Function checking wheather the provided variable contains a positive integer or not
function isInt() { local number_regexp="^[1-9][0-9]*$" ; if ! [[ $1 =~ ${number_regexp} ]] ; then echo "$1 Is not a valid positive integer" ; exit 2 ; fi ; }

#This function creates a random date between 01-01-2000 - 31-12-2020 in the form DD-MM-YYY 
function createDate() { local day=$RANDOM; let "day = (day % 30) + 1"; local month=$RANDOM; let "month = (month % 12) + 1"; local year=$RANDOM; let "year = (year % 20) + 2000"; 
                        if [[ ${day} -lt 10 ]]; then local string_day="0${day}"; else local string_day="${day}"; fi;
                        if [[ ${month} -lt 10 ]]; then local string_month="0${month}"; else local string_month="${month}"; fi; 
                        date="${string_day}-${string_month}-${year}"; }

#This function creates a random series of letters for each one of first name and last name parameters
function createName() { local fname_length=$RANDOM; let "fname_length = (fname_length % 9) + 3"; local lname_length=$RANDOM; let "lname_length = (lname_length % 9) + 3"; 
                        local alphabet=abcdefghijklmnopqrstuvwxyz;
                        fname="";
                        for l in $(seq 1 ${fname_length}); do local letter=$RANDOM; let "letter = (letter % 26)"; fname="${fname}${alphabet:$letter:1}"; done; 
                        lname=""; 
                        for l in $(seq 1 ${lname_length}); do local letter=$RANDOM; let "letter = (letter % 26)"; lname="${lname}${alphabet:$letter:1}"; done; }

#This function creates a record following the specifications given in the project description
function createRecord() { local id=$1; local diseases_num=${#diseases_array[*]}; local disease=${diseases_array[$RANDOM%diseases_num]}; 
                          createName; local age=$RANDOM; let "age = (age % 120  ) + 1";
                          local status=${patientStatus[$RANDOM%2]};
                          if [[ ${status} == "ENTER" ]]; then record="${id} ${status} ${fname} ${lname} ${disease} ${age}"; IFS=$'\n' countryPatients+=( ${record} ); else 
                          if [[ ${#countryPatients[@]} -eq 0 ]]; then record="${id} ${status} ${fname} ${lname} ${disease} ${age}"; else
                          local country_patients_num=${#countryPatients[@]}; record=${countryPatients[$RANDOM%country_patients_num]};
                          record=${record/ENTER/EXIT}; fi; fi; }

#Check wheather number of arguments provided is correct else end the script
if [ $# -ne 5 ]; then
    echo ${USAGE};
    exit 1;
fi

#File containing disease names
disease_file=$1
#File containing countries names
countries_file=$2
#Name of parent directory
parent_dir=$3
#Number of Files per sub-directory
files_num=$4
#Number of Records per File
records_num=$5

#Check if numFilesPerDirectory argument is a positive integer 
isInt ${files_num};
#Check if numRecordsPerFile argument is a positive integer
isInt ${records_num};

#Read diseasesFile and save contents into DISEASES variable
diseases="`cat ${disease_file}`"
#Save the contents of the variable into an array
declare -a diseases_array=( $diseases );
#Read countriesFile and save contents into COUNTRIES variable
countries="`cat ${countries_file}`"
#Save the contents of the variable into an array
declare -a countries_array=( $countries );

#Create the parent directory
mkdir ./${parent_dir};
#If a folder with the same name exists then end the script
if [ $? -ne 0 ]; then
    exit 3
fi

#Array containing ENTER and EXIT values for use in createRecordFunction
declare -a patientStatus=( ENTER EXIT );

patientNum=1;

#For each one of the countries listed in the countriesFile
for country in $countries; do
    #Create the country subDirectory
    mkdir ./${parent_dir}/${country};
    #Create numFilePerDIrectory Files
    #Array containing each patient record of a certain country
    declare -a countryPatients;
    for i in $(seq 1 ${files_num}); do
        #Find a date that does not exist in this folder
        createDate;
        while [ -f ./${parent_dir}/${country}/${date}.txt ]; do
            createDate;
        done;
        #Create a txt file with this date
        touch ./${parent_dir}/${country}/${date}.txt ;
        #For numRecordsPerFile Records
        for j in $(seq 1 ${records_num}); do
            createRecord $patientNum ${diseases_array};
            echo ${record} >> ./${parent_dir}/${country}/${date}.txt;
            let "patientNum++";
        done;
    done;
    unset countryPatients;
done;