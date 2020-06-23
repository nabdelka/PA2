#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "constants.h"
#include "functions.h"

// Global variables
int schedule_type = UNINITIALIZED;
int size = -1;
int weight = 0;
char Sadd[MAX_IP_LENGTH+1], Dadd[MAX_IP_LENGTH+1];
unsigned int Sport = 0, Dport = 0;
long int PktID = 0, Time = 0, Length = 0;
long int Timer = -1;
unsigned int work_index=-1; 
int work_weight = 0;  
long int work_Length = 0;
// Debug
int Iteration = 2;
bool iter = false;
long int print_time = 130809;
bool print_stop = false;
// File names
char *input_file = NULL;
char *stat_file = NULL;
char *output_file = NULL;
FILE *out_filePointer;

// Global array
flow_struct flows_array[MAX_FLOWS_NUM];
int flows_number = 0;

int schedule() {

	// Open file for read
	FILE *in_filePointer;
	errno_t err;
	err = fopen_s(&in_filePointer, input_file, "r");
	if (err != 0)
	{
		printf("-E- error while openning input file for reading\n");
		return -1;
	}
	err = fopen_s(&out_filePointer, output_file, "w");
	if (err != 0)
	{
		printf("-E- error while openning output file for writing\n");
		return -1;
	}
	

	// Declare & Init variables
	char c;
	int space = 0, Saddindex = 0, Daddindex = 0, index_to_add;
	char Sadd[MAX_IP_LENGTH+1], Dadd[MAX_IP_LENGTH+1];
	long int PktID = 0, Time = 0, Length = 0;
	unsigned int Sport = 0, Dport = 0;

	// Read file
	while ((c = getc(in_filePointer)) != EOF) {

		//printf("space = %d got char: %c\n", space, c);

		// restart for the next packet//
		if (c == '\n') {
			//printf("-I- [%d] Got packet %ld at time %ld length: %ld %s %s %d %d - w = %d\n",Timer,PktID, Time, Length, Sadd, Dadd,Sport,Dport,weight);

			if (Time > Timer) {
				if (flows_number == 0) { // Initial case - first line handling

					Timer = Time;
					//Pointer for next packet
					packet_struct* packet_ptr;
					packet_ptr = create_packet(PktID, Time, Length);
					index_to_add = get_flow_index(weight, Sadd, Dadd, Sport, Dport);
					add_packet_to_buf(index_to_add, packet_ptr);

					//continue reading
					goto continue_reading;
				}
				else {

					do {
						// time ITERATION
						if (Timer == print_time) {
							print_flows_array();
							//if(print_stop) return 0;
						}

						if(schedule_type== RR_TYPE)	WRR_func();
						else						DRR_func();

						if (Timer == print_time) {
							print_flows_array();
							if (print_stop) return 0;
						}
						Timer++;
					} while (Time > Timer);

					//Pointer for next packer
					packet_struct* packet_ptr;
					packet_ptr = create_packet(PktID, Time, Length);
					index_to_add = get_flow_index(weight, Sadd, Dadd, Sport, Dport);
					add_packet_to_buf(index_to_add, packet_ptr);

					// continue reading
					Iteration--;
					if (Iteration == 0 && iter)
					{
						print_flows_array();
						printf("fuck you\n");
						return -1;
					}
					goto continue_reading;

					
					
				}



			}
			else {
			// add to buffer
				//Pointer for next packer
				packet_struct* packet_ptr;
				packet_ptr = create_packet(PktID, Time, Length);

				index_to_add = get_flow_index(weight, Sadd, Dadd, Sport, Dport);
				add_packet_to_buf(index_to_add, packet_ptr);
			//continue reading
				goto continue_reading;
			}


			
			continue_reading:
			// Init for a new line 
			space = 0; Saddindex = 0; Daddindex = 0;
			Sadd[0] = 0; Dadd[0] = 0;
			PktID = 0; Time = 0; Length = 0; weight = 0;
			Sport = 0; Dport = 0;
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


	while (work_index != -1) {
		if (Timer == print_time) {
			print_flows_array();
			//return 0;
		}
		if (schedule_type == RR_TYPE)	WRR_func();
		else						    DRR_func();
		if (Timer == print_time) {
			print_flows_array();
			//return 0;
		}
		Timer++;
	}

	// Print Statistic
	// Open file for write
	FILE *stat_filePointer;
	err = fopen_s(&stat_filePointer, stat_file, "w");
	if (err != 0)
	{
		printf("-E- error while openning stat file for writing\n");
		return -1;
	}
	// write to stat file
	for (int i = 0; i < flows_number; i++) {
		float avg_delay = flows_array[i].sum_delay /flows_array[i].numPkts;
		fprintf(stat_filePointer, "%s %d %s %d %d %ld %.2f\n", flows_array[i].Sadd, flows_array[i].Sport, flows_array[i].Dadd, flows_array[i].Dport, flows_array[i].numPkts, flows_array[i].max_delay,avg_delay);
	}




	// close files
	if (fclose(in_filePointer) != 0) {
		printf("-E- error while closing input file\n");
		return -1;
	}
	if (fclose(out_filePointer) != 0) {
		printf("-E- error while closing output file\n");
		return -1;
	}
	if (fclose(stat_filePointer) != 0) {
		printf("-E- error while closing stat file\n");
		return -1;
	}
	return 0;
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


// Flows & packets
int get_flow_index( int weight_, char *sadd, char *dadd, unsigned int sport, unsigned int dport) {
	int i = 0, scom, dcom;
	for (i = 0; i < flows_number; i++) {
		scom = strcmp(sadd, flows_array[i].Sadd);
		dcom = strcmp(dadd, flows_array[i].Dadd);
		if (scom == 0 && dcom == 0 && sport == flows_array[i].Sport && dport == flows_array[i].Dport) return i; // The flow is exist
	}

	if (i == flows_number) {
		strcpy_s(flows_array[i].Sadd, sizeof(flows_array[i].Sadd), sadd);
		strcpy_s(flows_array[i].Dadd, sizeof(flows_array[i].Dadd), dadd);
		flows_array[i].Sport = sport;
		flows_array[i].Dport = dport;
		flows_array[i].weight = (weight_ == 0) ? size : weight_; // default weight

		// Init
		flows_array[i].numPkts = 0;
		flows_array[i].max_delay = 0;
		flows_array[i].sum_delay = 0;
		flows_array[i].credit = 0;

		flows_number++;		
	}
	else printf("-E- Unexpected flow index\n");

	return i;
}
// function that create the next packet //
packet_struct* create_packet(long int pktid, long int time, long int length) {
	packet_struct* packet_ptr = (packet_struct*)malloc(sizeof(packet_struct));
	// TODO check if null
	packet_ptr->Length = length;
	packet_ptr->Time = time;
	packet_ptr->PktID = pktid;
	packet_ptr->next = NULL;
	return packet_ptr;
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
	flows_array[index].numPkts++;
}


void WRR_func() {
	packet_struct* packet_in_work;
	packet_struct* dirty_packet;
	unsigned int i = 0;
	bool out_packet = false;
	//printf("Started WRR: %d %d %d\n",work_Length,work_weight,work_index);
	long int delay;

	if (work_index == -1) {
		printf("Noor fucking first call %d\n",flows_number);
		for(i = 0; i< flows_number;i++){
			if (flows_array[i].head != NULL) {
				work_index = i;
				work_weight = flows_array[i].weight;
				packet_in_work = flows_array[work_index].head;
				work_Length = packet_in_work->Length;
				// update delay stat
				delay = Timer - packet_in_work->Time;
				if (delay > flows_array[i].max_delay) flows_array[i].max_delay = delay;
				flows_array[i].sum_delay += delay;
				fprintf(out_filePointer, "%ld: %ld\n",Timer, packet_in_work->PktID);
				break;
			}
		}
	}
	packet_in_work = flows_array[work_index].head;

	if (work_Length != 0 && work_weight != 0 && work_index != -1) {
		work_Length--;
	}
	if (work_Length == 0 && work_weight != 0 && work_index != -1) {
		out_packet = true;
		// remove out packet
		dirty_packet = flows_array[work_index].head;
		flows_array[work_index].head = dirty_packet->next;
		free(dirty_packet);
		work_weight--;


	}


	if ((work_weight == 0 && work_index != -1) || flows_array[work_index].head == NULL) {
		for (i = work_index + 1; i <= flows_number; i++) {
			if (i == flows_number) i = 0; // cyclic
			
			if (flows_array[i].head != NULL) {
				work_index = i;
				work_weight = flows_array[i].weight;
				work_Length = packet_in_work->Length;
				break;
			}
			else if (i == work_index) {
				work_index = -1;
				break;
			}
		}
	}

	if (out_packet && work_index!=-1) {
		packet_in_work = flows_array[work_index].head;
		if (packet_in_work == NULL) {
			printf("WTF %d\n",work_index);
			print_flows_array();
		}
		work_Length = packet_in_work->Length;
		// update delay stat
		delay = Timer - packet_in_work->Time + 1;
		if (delay > flows_array[i].max_delay) flows_array[i].max_delay = delay;
		flows_array[i].sum_delay += delay;

		fprintf(out_filePointer, "%ld: %ld\n", Timer+1, packet_in_work->PktID);
	}

}



void DRR_func() {
	packet_struct* current_packet;

	if (work_Length > 0) {
		// Sending a packet - wait till finish
		work_Length--;
		return;
	}
	if (work_index == -1) {
		// Initial case init for DRR
		work_index = 0;
	}
	bool new_cycle = false;
	for (int i = work_index; i <= flows_number; i++) {
		if (i == flows_number) {
			// TODO need to identify empty buffer
			i = 0; new_cycle = true;
		}
		if (flows_array[i].head == NULL) {
			// Nothing to send
			if (Timer == print_time && i == 3) printf("reset credit for 3\n", flows_array[i].credit);

			flows_array[i].credit = 0;
			continue;
		}
		else {
			if ((i != work_index) || new_cycle) {
				if (Timer == print_time && i == 3) printf("work_idx = %d   updating credit of flow 3\n%ld ->", work_index,flows_array[i].credit);
				flows_array[i].credit = flows_array[i].credit + flows_array[i].weight;
				if (Timer == print_time && i == 3) printf(" %ld\n", flows_array[i].credit);

			}
			current_packet = flows_array[i].head;
			if (flows_array[i].credit >= current_packet->Length) {
				work_index = i;
				fprintf(out_filePointer, "%ld: %ld\n", Timer, current_packet->PktID);
				flows_array[i].credit = flows_array[i].credit - current_packet->Length;
				work_Length = current_packet->Length-1;
				flows_array[i].head = current_packet->next;
				free(current_packet);
				return;
			}
		}
	}
}




// Debug
void print_flows_array() {
	printf("We have %d flows at time %d.\n", flows_number,Timer);
	for (int i = 0; i < flows_number; i++) {
		if (flows_array[i].head != NULL) printf("# Flow at index %d - W = %d:\n", i, flows_array[i].weight);
		else {
			printf("# Flow at index %d has no packets\n\n", i);
			continue;
		}
		printf("# Credits = %ld\n", flows_array[i].credit);
		packet_struct* packet_ptr = flows_array[i].head;
		do {
			printf("   \% PktID = %d    Length = %d\n", packet_ptr->PktID, packet_ptr->Length);
			packet_ptr = packet_ptr->next;

		} while (packet_ptr != NULL);
		printf("\n");
	}

}

