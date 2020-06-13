#pragma once

// struct for flow index
typedef struct flow_index_struct
{
	char Sadd_index[15], Dadd_index[15];
	unsigned int Sport_index, Dport_index;
	struct flow_index_struct *head;
} flow_index_str;


// struct for packet
typedef struct packet_sturct
{
	long int PktID, Time, Length;
	int weight;
	struct packet_sturct *next;
} flow_packet;


void readfile(FILE *filePointer);
bool check_args_valid();
void get_sch_type(char *sch_type_str);
void get_size(char *size_str);
void get_file_names(char *name_str);
flow_index_str* ceate_flow(char *sadd, char *dadd, unsigned int sport, unsigned int dport, int daddindex, int saddindex);
flow_packet* ceate_packet(long int pktid, long int time, long int length, int weight_);
