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
unsigned int Timer = 0;
unsigned int work_index=-1; // TODO fix
int work_weight = 0; // TODO fix
long int work_Length = 0;

// File names
char *input_file = NULL;
char *stat_file = NULL;
char *output_file = NULL;

// Global array
flow_struct flows_array[MAX_FLOWS_NUM];
int flows_number = 0;

int schedule_wrr() {

	// Open file for read
	FILE *filePointer;
	errno_t err;
	err = fopen_s(&filePointer, input_file, "r");
	if (err != 0)
	{
		printf("-E- error while openning input file for reading\n");
		return -1;
	}

	// Declare & Init variables
	char c;
	int space = 0, Saddindex = 0, Daddindex = 0, index_to_add;
	char Sadd[MAX_IP_LENGTH+1], Dadd[MAX_IP_LENGTH+1];
	long int PktID = 0, Time = 0, Length = 0;
	unsigned int Sport = 0, Dport = 0;

	// Read file
	while ((c = getc(filePointer)) != EOF) {

		printf("space = %d got char: %c\n", space, c);

		// restart for the next packet//
		if (c == '\n') {
			printf("-I- Got packet %ld at time %ld length: %ld %s %s %d %d\n",PktID, Time, Length, Sadd, Dadd,Sport,Dport);
			if (Time > Timer) {
				// empty buffer
				// if still > timer
					//Timer = Time; // just at firs packet
					printf("-OUT- %d: %ld\n", Timer, PktID);
					//Timer += Length;
					//continue reading
				// else
					//
			}
			else {
				// add to buffer
				//continue reading
			}

			printf("allocating memory\n");

			//Pointer for next flow
			flow_struct* flow_ptr;
			flow_ptr = create_flow(weight, Sadd, Dadd, Sport, Dport);
			//Pointer for next packer
			packet_struct* packet_ptr;
			packet_ptr = create_packet(PktID, Time, Length);
			//check flow and add / add packer//
			printf("memory allocated\n");
			// add backet to appropriate flow and add flow//
			if (packet_ptr->Time = Timer) {
				index_to_add = add_flow_to_buf(flow_ptr);
				add_packet_to_buf(index_to_add, packet_ptr);
			}
			while (packet_ptr->Time >= Timer) {
				printf("entered while\n");

				///Work in the flow///
				WRR_func();
				printf("WRR_func end\n");

			}
			

			// Init for a new line 
			space = 0; Saddindex = 0; Daddindex = 0;
			Sadd[0] = 0; Dadd[0] = 0;
			PktID = 0; Time = 0; Length = 0; weight = 0;
			Sport = 0; Dport = 0;
			if (packet_ptr->Time != Timer - 1) {
				Timer = Timer + 1;
			}
			continue;
		}
		// Read packet ID
		if (c != ' ' && space == 0) {
			PktID = PktID * 10 + (c - '0');
			continue;
		}
		if (c == ' ' && space == 0) {
			space = 1;
			continue;
		}
		// Read arrival time
		if (c != ' ' && space == 1) {
			Time = Time * 10 + (c - '0');
			continue;
		}
		if (c == ' ' && space == 1) {
			space = 2;
			continue;
		}
		// Read source address
		if (c != ' ' && space == 2) {
			Sadd[Saddindex] = c;
			Saddindex++;
			continue;
		}
		if (c == ' ' && space == 2) {
			Sadd[Saddindex] = 0;
			space = 3;
			continue;
		}
		// Read source port
		if (c != ' ' && space == 3) {
			Sport = Sport * 10 + (c - '0');
			continue;
		}
		if (c == ' ' && space == 3) {
			space = 4;
			continue;
		}
		// Read destination address
		if (c != ' ' && space == 4) {
			Dadd[Daddindex] = c;
			Daddindex++;
			continue;
		}
		if (c == ' ' && space == 4) {
			Dadd[Daddindex] = 0;
			space = 5;
			continue;
		}
		// Read destination port
		if (c != ' ' && space == 5) {
			Dport = Dport * 10 + (c - '0');
			continue;
		}
		if (c == ' ' && space == 5) {
			space = 6;
			continue;
		}
		// Read packet length
		if (c != ' ' && space == 6) {
			Length = Length * 10 + (c - '0');
			continue;
		}
		if (c == ' ' && space == 6) {
			space = 7;
			continue;
		}
		// Read weight (if need)
		if (c != ' ' && space == 7) {
			weight = weight * 10 + (c - '0');
			continue;
		}
		if (c == ' ' && space == 7) {
			space = 8;
			continue;
		}

		
	}

	// close input file
	if (fclose(filePointer) != 0) {
		printf("-E- error while closing input file for reading\n");
		return -1;
	}
}


int schedule() {
	// check schedule type: WRR/DRR and call the relevand scheduler
	if (schedule_type == RR_TYPE) return schedule_wrr();
		
	else if(schedule_type == DRR_TYPE){
		// TODO implement DRR func

	}
	else {
		printf("-E- Unexpected schedule type\n");
		return -1;
	}
}
// function that create the next flow //
flow_struct* create_flow(int weight_ , char *sadd, char *dadd, unsigned int sport, unsigned int dport ) {


	flow_struct* flow_ptr = (flow_struct*)malloc(sizeof(flow_struct)); 	// TODO check if null

	errno_t err;
	err = strcpy_s(flow_ptr->Sadd_index, MAX_IP_LENGTH + 1, sadd); // TODO check err
	if (err != 0) printf("Err copy 1\n");
	err = strcpy_s(flow_ptr->Dadd_index, MAX_IP_LENGTH + 1, dadd); // TODO check err
	if (err != 0) printf("Err copy 2\n");


	flow_ptr->Dport_index = dport;
	flow_ptr->Sport_index = sport;
	flow_ptr->head = NULL;
	flow_ptr->weight = weight_;
	return flow_ptr;
}


// function that create the next packet //
packet_struct* create_packet(long int pktid , long int time , long int length) {
	packet_struct* packet_ptr = (packet_struct*)malloc(sizeof(packet_struct));
	// TODO check if null
	packet_ptr->Length = length;
	packet_ptr->Time = time;
	packet_ptr->PktID = pktid;
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



int add_flow_to_buf(flow_struct *flow_pointer) {
	int i = 0, equal = 0, scom, dcom;
	for (i = 0; i < flows_number; i++) {
		scom = strcmp(flow_pointer->Sadd_index, flows_array[i].Sadd_index);
		dcom = strcmp(flow_pointer->Dadd_index, flows_array[i].Dadd_index);
		if (scom == 0 && dcom == 0 && flow_pointer->Sport_index == flows_array[i].Sport_index && flow_pointer->Dport_index == flows_array[i].Dport_index) {
			equal = 1;
		}
		break;
	}
	if (i == flows_number) {
		strcpy_s(flows_array[i].Sadd_index, sizeof(flow_pointer->Sadd_index), flow_pointer->Sadd_index);
		strcpy_s(flows_array[i].Dadd_index, sizeof(flow_pointer->Dadd_index), flow_pointer->Dadd_index);
		flows_array[i].Sport_index = flow_pointer->Sport_index;
		flows_array[i].Dport_index = flow_pointer->Dport_index;
		flows_number++;
		if (flows_array[i].weight == 0) flows_array[i].weight = size;
		else							flows_array[i].weight = flow_pointer->weight;
		
	}

	return i;
}

void add_packet_to_buf(int index, packet_struct* packet_to_add) {
	packet_struct* packet_in_chain;
	if (flows_array[index].head == NULL) {
		flows_array[index].head = packet_to_add;
	}
	else {
		packet_in_chain = flows_array[index].head;
		while(packet_in_chain->next != NULL){
			packet_in_chain = packet_in_chain->next;
		}
		packet_in_chain->next = packet_to_add;
	}
}


void WRR_func() {
	packet_struct* packet_in_work;
	unsigned int i = 0;
	if (work_index = -1) {
		while (i < sizeof(flows_array)) {
			if (flows_array[i].head != NULL) {
				work_index = i;
				work_weight = flows_array[i].weight;
				packet_in_work = flows_array[work_index].head;
				work_Length = packet_in_work->Length;
				break;
			}
		}
	}
	if (work_weight == 0 && work_index != -1) {
		for (i = work_index + 1; i < flows_number; i++) {
			packet_in_work = flows_array[i].head;
			if (packet_in_work != NULL) {
				work_index = i;
				work_weight = flows_array[i].weight;
				work_Length = packet_in_work->Length;
				break;
			}
			else if (i == flows_number) {
				i = 0;
			}
			else if (i = work_index) {
				work_index = -1;
				break;
			}
		}
	}
	if (work_Length == 0 && work_weight != 0 && work_index != -1) {
		flows_array[work_index].head = packet_in_work->next;
		packet_in_work = flows_array[i].head;
		work_Length = packet_in_work->Length;
		work_weight--;
	}
	if (work_Length != 0 && work_weight !=0 && work_index != -1) {
		///////////// print in the file ////////////////////////////
		work_Length--;
		Timer++;
	}
}

