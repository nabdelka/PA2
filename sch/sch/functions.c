#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "constants.h"

// Global variables
int schedule_type = UNINITIALIZED;
int size = -1;
int weight = 0;

// File names
char *input_file = NULL;
char *stat_file = NULL;
char *output_file = NULL;

void readfile(FILE *filePointer) {
	char c;
	int space = 0, Saddindex = 0, Daddindex = 0, line = 0;
	char Sadd[15], Dadd[15];
	long int PktID = 0, Time = 0, Length = 0;
	unsigned int Sport = 0, Dport = 0;
	while ((c = getc(filePointer)) != EOF) {
		while (c != '\n') {
			if (c != ' ' && space == 0) {
				PktID = PktID * 10 + (c - '0');
			}
			if (c == ' ' && space == 0) {
				space = 1;
			}
			if (c != ' ' && space == 1) {
				Time = Time * 10 + (c - '0');
			}
			if (c == ' ' && space == 1) {
				space = 2;
			}
			if (c != ' ' && space == 2) {
				Sadd[Saddindex] = c;
				Saddindex++;
			}
			if (c == ' ' && space == 2) {
				space = 3;
			}
			if (c != ' ' && space == 3) {
				Sport = Sport * 10 + (c - '0');
			}
			if (c == ' ' && space == 3) {
				space = 4;
			}

			if (c != ' ' && space == 4) {
				Dadd[Daddindex] = c;
				Daddindex++;
			}
			if (c == ' ' && space == 4) {
				space = 5;
			}
			if (c != ' ' && space == 5) {
				Dport = Dport * 10 + (c - '0');
			}
			if (c == ' ' && space == 5) {
				space = 6;
			}
			if (c != ' ' && space == 6) {
				Length = Length * 10 + (c - '0');
			}
			if (c == ' ' && space == 6) {
				space = 7;
			}
			if (line == 0 && (c != ' ' && space == 7)) {
				weight = weight * 10 + (c - '0');
			}
		}
		// at this point the values of each field is readed //

		// restart for the next row//
		if (c == '\n') {
			int space = 0, Saddindex = 0, Daddindex = 0, line = 1;
			char Sadd[15], Dadd[15];
			long int PktID = 0, Time = 0, Length = 0, weight = 0;
			unsigned int Sport = 0, Dport = 0;
		}
	}
	if (weight == 0) {
		weight = size;
	}
}
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
	}
	else if (strcmp(sch_type_str, "DRR") == 0) {
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
	input_file = (char*)malloc(name_len + 8);
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
		strcpy_s(input_file, name_len + 1, name_str);
		strcpy_s(stat_file, name_len + 1, name_str);
		strcpy_s(output_file, name_len + 1, name_str);
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