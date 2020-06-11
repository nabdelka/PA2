#include <stdio.h>

int main() {
	FILE *filePointer;
	filePointer = fopen("fileName.txt", "r");
	return 0;

}

// TODO N - function (or 3) takes "name" and returns file_name as str
// TODO A - convert lines in input file to int fields
void readfile(FILE *filePointer) {
	char c;
	int space = 0 , Saddindex=0 ,Daddindex=0;
	char Sadd[15] , Dadd[15];
	long int PktID=0 , Time=0 , Length=0 , weight=0;
	unsigned int Sport=0 , Dport=0;
	while ((c = getc(filePointer)) != EOF) {
		while (c != '\n') {
			if (c =! ' ' && space == 0) {
				PktID = PktID * 10 + (c - '0');
			}
			if (c == ' ' && space == 0) {
				space = 1;
			}
			if (c =! ' ' && space == 1) {
				Time = Time * 10 + (c - '0');
			}
			if (c == ' ' && space == 1) {
				space = 2;
			}
			if (c =! ' ' && space == 2) {
				Sadd[Saddindex] = c;
				Saddindex++;
			}
			if (c == ' ' && space == 2) {
				space = 3;
			}
			if (c = !' ' && space == 3) {
				Sport = Sport * 10 + (c - '0');
			}
			if (c == ' ' && space == 3) {
				space = 4;
			}

			if (c = !' ' && space == 4) {
				Dadd[Daddindex] = c;
				Daddindex++;
			}
			if (c == ' ' && space == 4) {
				space = 5;
			}
			if (c = !' ' && space == 5) {
				Dport = Dport * 10 + (c - '0');
			}
			if (c == ' ' && space == 5) {
				space = 6;
			}
			if (c = !' ' && space == 6) {
				Length = Length * 10 + (c - '0');
			}
			if (c == ' ' && space == 6) {
				space = 7;
			}
			if (c = !' ' && space == 7) {
				weight = weight * 10 + (c - '0');
			}
		}
		/// at this point the values of each field is readed //

		// restart for the next row//
		if (c == '\n') {
			int space = 0, Saddindex = 0, Daddindex = 0;
			char Sadd[15], Dadd[15];
			long int PktID = 0, Time = 0, Length = 0, weight = 0;
			unsigned int Sport = 0, Dport = 0;
		}
	}
}



// TODO N - take from command line: RR/DRR and size, save in global variables
// TODO A - check if have wieght in first line - global variable