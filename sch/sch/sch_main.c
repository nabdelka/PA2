#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "constants.h"


// Global variables
int schedule_type = UNINITIALIZED;
int size = -1;

// File names
char *input_file = NULL;
char *stat_file = NULL;
char *output_file = NULL;


bool check_args_valid() {
	bool status_good = true;
	if (schedule_type == UNINITIALIZED) {
		printf("-E- schedule type error\n");
		status_good = false;
	}
	if (size <= 0) {
		printf("-E- size error\n");
		status_good = false;

	}
	

	return status_good;

}
void get_sch_type(char *sch_type_str) {

	if (strcmp(sch_type_str, "RR") == 0) {
		schedule_type = RR_TYPE;
		return;
	}else if (strcmp(sch_type_str, "DRR") == 0) {
		schedule_type = DRR_TYPE;
		return;
	}
	else {
		schedule_type = UNINITIALIZED;
		return;
	}

}
void get_size(char *size_str) {

	size = atoi(size_str);
	printf("Got size %d\n", size);

}
void get_file_names(char *name_str) {
	int name_len = strlen(name_str);
	// Dynamic memory allocation
	input_file = (char*)malloc(name_len+8);
	stat_file = (char*)malloc(name_len + 10);
	output_file = (char*)malloc(name_len + 9);
	// Checking allocation
	bool status_good = true;
	if (input_file == NULL) {
		printf("-E- input file allocation error\n");
		status_good = false;

	}
	if (stat_file == NULL) {
		printf("-E- stat file allocation error\n");
		status_good = false;

	}
	if (output_file == NULL) {
		printf("-E- output file allocation error\n");
		status_good = false;

	}
	if (status_good) {

		// Adding name
		strcpy_s(input_file, name_len+1, name_str);
		strcpy_s(stat_file, name_len+1, name_str);
		strcpy_s(output_file, name_len+1, name_str);
		// Adding files extentions 
		strcat_s(input_file, name_len + 8, "_in.txt");
		strcat_s(stat_file, name_len + 10, "_stat.txt");
		strcat_s(output_file, name_len + 9, "_out.txt");
		// TODO remove prints
		printf("-I- input = %s\n", input_file);
		printf("-I- stat = %s\n", stat_file);
		printf("-I- output = %s\n", output_file);

	}
	
}
int main(int argc, char *argv[]) {

	if (argc != 4) {
		printf("Please provide arguments\n");
		return -1;
	}
	// Get input arguments and check validity
	get_sch_type(argv[1]);
	get_size(argv[3]);
	if (!check_args_valid()) return -1;
	// Generate file names
	get_file_names(argv[2]);



	printf("Weeeeeee\n");
	return 0;
}

// TODO N Done - function (or 3) takes "name" and returns file_name as str
// TODO A - convert lines in input file to int fields
// TODO N Done - take from command line: RR/DRR and size, save in global variables
// TODO A - check if have wieght in first line - global variable