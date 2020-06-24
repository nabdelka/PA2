#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "constants.h"
#include "functions.h"



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

	schedule();



	printf("Weeeeeee\n");
	return 0;
}



