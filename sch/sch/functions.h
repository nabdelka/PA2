#pragma once

// struct for flow index
typedef struct flow_struct
{
	char Sadd[MAX_IP_LENGTH+1], Dadd[MAX_IP_LENGTH+1];
	unsigned int Sport, Dport;
	int weight;
	// DRR
	long int credit;
	//stat
	long int max_delay;
	long int sum_delay;
	long int max_buf ;
	long int sum_buf;
	long int current_backets_num ;
	int numPkts;
	// List
	struct flow_index_struct *head;
} flow_struct;


// struct for packet
typedef struct packet_sturct
{
	long int PktID, Time, Length;
	struct packet_sturct *next;
} packet_struct;


int schedule();
bool check_args_valid();
void get_sch_type(char *sch_type_str);
void get_size(char *size_str);
bool get_file_names(char *name_str);
packet_struct* create_packet(long int pktid, long int time, long int length);
int get_flow_index(int weight_, char *sadd, char *dadd, unsigned int sport, unsigned int dport);
void add_packet_to_buf(int index, packet_struct* packet_to_add);
void WRR_iteration();
void DRR_iteration();

void print_flows_array();
bool buffer_is_empty();
void update_packets_num();