#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "constants.h"
#include "functions.h"



int main(int argc, char *argv[]) {

	if (argc != 4) {
		printf("Usage: sch.exe <sch_type> <name> \n");
		return -1;
	}

	// Get input arguments and check validity
	get_sch_type(argv[1]);
	get_size(argv[3]);
	if (!check_args_valid()) return -1;

	// Generate file names
	if (get_file_names(argv[2])) return schedule();
	else return -1;
}



