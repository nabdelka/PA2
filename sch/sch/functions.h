#pragma once

// struct for flow index
typedef struct flow_struct
{
	char Sadd_index[MAX_IP_LENGTH+1], Dadd_index[MAX_IP_LENGTH+1];
	unsigned int Sport_index, Dport_index;
	int weight;
	int numPkts;
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
flow_struct* create_flow(int weight_ ,char *sadd, char *dadd, unsigned int sport, unsigned int dport);
packet_struct* create_packet(long int pktid, long int time, long int length);
int add_flow_to_buf(flow_struct *flow_pointer);
void add_packet_to_buf(int index, packet_struct* packet_to_add);
void WRR_func();
void print_flows_array();
