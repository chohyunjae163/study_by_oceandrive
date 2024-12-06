#include <stdio.h>
#include <stdlib.h>

int main() {

	//open a text file
	FILE* file = NULL;
	file = fopen( "list.txt", "r" );
	if ( file == NULL ) {
		printf("Error in creating file!");
		return 1;
	}

	//read and output a list of options
	char line[50];
	char chosen_line[50];
	
	srand(time(NULL));
	const int NUM_OPTIONS = 8;
	int rand_i = rand() % NUM_OPTIONS;
	int count = 0;

	printf("\nList of Options");
	printf("\n===================\n");
	
	while (fgets (line , sizeof(line) , file)) {
		if (rand_i == count) {
			strcpy(chosen_line, line);
		}
		count++;
		printf("%d - %s",count, line);
		
	}

	printf("\n===================\n");
	//output the chosen one
	printf("random pick : %s", chosen_line);
	
	fclose(file);
	printf("\nfile closed.");
	return 0;
}
