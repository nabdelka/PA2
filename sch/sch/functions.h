#pragma once

// struct for flow index
typedef struct flow_struct
{
	char Sadd_index[MAX_IP_LENGTH+1], Dadd_index[MAX_IP_LENGTH+1];
	unsigned int Sport_index, Dport_index;
	int weight;
	//stat
	long int max_delay;
	long int sum_delay;
	int numPkts;
	long int credit;
	struct flow_index_struct *head;
} flow_struct;


// struct for packet
typedef struct packet_sturct
{
	long int PktID, Time, Length;
	struct packet_sturct *next;
} packet_struct;


int schedule_wrr();
bool check_args_valid();
void get_sch_type(char *sch_type_str);
void get_size(char *size_str);
void get_file_names(char *name_str);
packet_struct* create_packet(long int pktid, long int time, long int length);
int add_flow_to_buf(int weight_, char *sadd, char *dadd, unsigned int sport, unsigned int dport);
void add_packet_to_buf(int index, packet_struct* packet_to_add);
void WRR_func();
void DRR_func();
void DRR_func_try();

void print_flows_array();
