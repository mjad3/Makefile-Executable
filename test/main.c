#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "parser.h"


int main(int argc, char *argv[])	{
	// Sample input
	// make 

	// Acceptable Input Strings
	char* make_file = "./sample_makefile";
	char* Make_file = "./Makefile";
	char* target = "";

	// Make Target
	if(argv[1] != NULL)	{
		target = argv[1];
	}

	// Look for file
	if(access(make_file, F_OK ) != -1 ) {
    	

    	// Set up File Reader
    	FILE* file = fopen(make_file, "r");
    	char line[4000];

    	// Cycle through file
    	while (fgets(line, sizeof(line), file)) {
    		// Cycle til  we find our target
    		
    		// Find our main target
    		if(is_target(line, target))	{
    			// Build Dependency graph
    			printf("%s\n", line);

    			// recurse bottom up calling forking processes as you go
    			
    			exit(0);


    		}
    	}

    	// If this is the target we want lets execute code

    	fclose(file);

	} else if(access(Make_file, F_OK ) != -1 ) {
    	printf("File Found\n");
    	// Then we run it
	} else {
    	exit(1);
    	// THIS IS AN ERROR
	}

	return 0;
}
