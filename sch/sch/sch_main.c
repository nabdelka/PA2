#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "functions.h"
#include "constants.h"



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

	FILE *filePointer;
	errno_t err;

	// Open file for read 
	err = fopen_s(&filePointer, "fileName.txt", "r");
	if (err != 0)
	{
		printf("-E- error while openning input file for reading\n");
		return -1;
	}


	if (fclose(filePointer) != 0) {
		printf("-E- error while closing input file for reading\n");
		return -1;
	}


	printf("Weeeeeee\n");
	return 0;
}

// TODO N Done - move functions to a seperate files
// TODO N - global array to save flows with lists addresses and global int to store flows number
// TODO N Done - fix open_s
// TODO N - function to check if flows parameters (4 addresses) is exist - takes input ( sadd sport dadd dport ) return index if exist, and -1 if not
// TODO N - function takes index and a new packet address, and it adds the packet to the begining of the list
// TODO N - function to generate a new packet/flow struct and return pointer + free


// TODO A - add structs for packet and for flow
// TODO A Done - close opened file
// TODO A - function to add new packet (struct) to a flow:
//			if flow exist, add to the list (call the function below)
//			else flow and packet to the global array and updat flow num
// TODO A - fill array when reading packets


