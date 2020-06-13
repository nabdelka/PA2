#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "constants.h"
#include "functions.h"

// Global variables
int schedule_type = UNINITIALIZED;
int size = -1;
int weight = 0;
char Sadd[15], Dadd[15];
unsigned int Sport = 0, Dport = 0;
long int PktID = 0, Time = 0, Length = 0;

// File names
char *input_file = NULL;
char *stat_file = NULL;
char *output_file = NULL;

// Global array
flow_index_str flows_array[MAX_FLOWS_NUM];
int flows_number = 0;

void readfile(FILE *filePointer) {
	char c;
	int space = 0, Saddindex = 0, Daddindex = 0;
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
			if (c != ' ' && space == 7) {
				weight = weight * 10 + (c - '0');
			}
			if (c == ' ' && space == 7) {
				space = 8;
			}
			// at this point the values of each field is readed //
			//Pointer for next flow
			flow_index_str* flow_ptr;
			flow_ptr = ceate_flow(Sadd[0], Dadd[0], Sport, Dport, Daddindex, Saddindex);
			//Pointer for next packer
			flow_packet* packet_ptr;
			packet_ptr = ceate_packet(PktID, Time, Length, weight);
			//check flow and add / add packer//

			// restart for the next row//
			if (c == '\n') {
				int space = 0, Saddindex = 0, Daddindex = 0;
				char Sadd[15], Dadd[15];
				long int PktID = 0, Time = 0, Length = 0, weight = 0;
				unsigned int Sport = 0, Dport = 0;
			}
		}
	}
}

// function that create the next flow //

flow_index_str* ceate_flow(char *sadd, char *dadd, unsigned int sport, unsigned int dport ,int daddindex , int saddindex) {
	int i = 0;
	flow_index_str* flow_ptr = (flow_index_str*)malloc(sizeof(flow_index_str));
	while (i < daddindex+1) {
		flow_ptr->Dadd_index[i] = dadd[i];
	}
	while (i < saddindex + 1) {
		flow_ptr->Sadd_index[i] = sadd[i];
	}
	flow_ptr->Dport_index = dport;
	flow_ptr->Sport_index = sport;
	flow_ptr->head = NULL;
	return flow_ptr;
}


// function that create the next packet //

flow_packet* ceate_packet(long int pktid , long int time , long int length , int weight_) {
	flow_packet* packet_ptr = (flow_packet*)malloc(sizeof(flow_packet));
	packet_ptr->Length = length;
	packet_ptr->Time = time;
	packet_ptr->PktID = pktid;
	packet_ptr->weight = weight_;
	packet_ptr->next = NULL;
	return packet_ptr;
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