/////////////////////////////////////////////////////////////
// CS 537, Fall 2020
// Program 3, 537make
// Michael Sachen, 9073631716, sachen
// Matt Jadin, 9065235468, jadin
/////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include "representation.h"
#include "parser.h"
#include "build_specs.h"

int main(int argc, char * argv[]) {
  // Acceptable Input Strings
  char * make_file = "./makefile";
  char * Make_file = "./Makefile";
  char * target = "";
  int m = 0; //keeps track of whether or not we found a makefile
  // Look for -f here
  int f_spot = 0;
  if (argc > 4) {
    fprintf(stderr, "Too many arguments, use target name, and/or -f with makefile name\n");
    exit(1);
  }
  FILE * file;
  //check for -f flag and specified makefile
  for (int x = 0; x < argc; x++) {
    if (strcmp(argv[x], "-f") == 0) {
      if (access(argv[x + 1], F_OK) != -1) {
        file = fopen(argv[x + 1], "r");
        m = 1;
        f_spot = x;

      } else {
        fprintf(stderr, "Could not find specified makefile\n");
        exit(1);
      }
    }
  }
  
  //Check for specified target  
  if ((f_spot == 2 || f_spot == 0) && argv[1] != NULL) {
    target = argv[1];
  } else if (f_spot == 1 && argv[3] != NULL) {
    target = argv[3];
  }

  if (access(Make_file, F_OK) != -1 && m == 0) {
    file = fopen(Make_file, "r");
    m = 1;
  } else if (access(make_file, F_OK) != -1 && m == 0) {
    file = fopen(make_file, "r");
    m = 1;
  }

  // Look for file in current directory
  if (m == 1) {
    // Set up File Reader

    int input_size = 4096;
    char line[input_size];

    // Iterate to find number of targets
    int targ_count = 0;
    while (fgets(line, sizeof(line), file)) {

      if (line[0] == '#') continue;
      if (is_target(line) == 1) {
        targ_count++;
      }
    }
    // Reset Pointer
    rewind(file);

    // Create data structure for Nodes with number of targets
    struct node * n_arr[targ_count];

    // Set Target
    // Cycle through file
    int j = -1;
    int lineNum = 1;
    char temp[input_size];
    while (fgets(line, sizeof(line), file)) {
      // Check Line Length
      fgets(temp, sizeof(line), file);
      if (strlen(line) >= input_size - 1) {
        fprintf(stderr, "Line too long\n");
        exit(1);
      }

      // Check for comments
      if (line[0] == '#') {
        lineNum++;
        continue;
      }

      //Check for null byte in line
      //char c;
      //while ((c = fgetc(file)) != '\n' && c != EOF) {
      //if(c == '\0')
      //}

      //char* c = line;
      // while (*c) putchar(*c++);
      lineNum++;
      char t;
      for (int x = 0; x < sizeof(line); x++) {

        t = line[x];

        if (t == '\0') {
          if (fgets(temp, sizeof(line), file)) {
            fprintf(stderr, "%d: Null byte found: exiting\n", lineNum);
            exit(1);
          }
        }
        if (t == EOF || t == '\n') break;

      }

    }
    rewind(file);
    lineNum = 1; //double check this

    while (fgets(line, sizeof(line), file)) {
      //Check for comments, ignore them
      if (line[0] == '#') {
        lineNum++;
        continue;
      }
      // If Line is a Target
      if (is_target(line) == 1) {
        // Get all dependencies
        char deps[strlen(line)];
        snprintf(deps, strlen(line), "%s", line);
        char * token;
        token = strtok(deps, ":");
        j++;
        // Add Node to Data Structure
        n_arr[j] = CreateNode();

        // Trim Trailing space           
        strcpy(n_arr[j] -> target_name, removeWhiteSpace(token));

        // This is our dependencies as a char array
        // Set up Node
        if ((token = strtok(NULL, "/")) != NULL) {
          while (isspace((unsigned char) * token)) token++;
          char ** dep_array = malloc((4096 / 3) * sizeof(char));
          if (dep_array == NULL) {
            fprintf(stderr, "malloc error\n");
            exit(1);
          }
          n_arr[j] -> dsize = splitter(token, dep_array, lineNum);
          n_arr[j] -> depends = dep_array;
        }
      }
      // If Line is a Command
      else if (line[0] == '\t') {

        //check for tab
        if (n_arr[j] -> csize >= sizeof(n_arr[j] -> commands)) {
          char** temp = malloc(sizeof(char*) * (n_arr[j] -> csize * 2));
          for(int y = 0; y < n_arr[j] -> csize; y++){
            temp[y] = n_arr[j] -> commands[y];
          }
          n_arr[j] -> commands = temp;
        }

        line[0] = ' ';

        n_arr[j] -> commands[n_arr[j] -> csize] = malloc(sizeof(char) * input_size);
        if (n_arr[j] -> commands[n_arr[j] -> csize] == NULL) {
          fprintf(stderr, "malloc error\n");
          exit(1);
        }
        strcpy(n_arr[j] -> commands[n_arr[j] -> csize], line);
        n_arr[j] -> csize++;
      }

      // If Line is not a command target or blank i.e. an Error
      else if (line[0] != '\0' && line[0] != '\n' && line[0] != EOF) {
        //if line is not blank, throw error
        fprintf(stderr, "%d: Error: nonblank line withough target or tab\n", lineNum);
        exit(1);
      }

      // Increment Line number
      lineNum++;
    } // END OF MAIN WHILE LOOP

    //build list of Targets for a node

    char ** tlist = malloc(targ_count * sizeof(char * ));
    if (tlist == NULL) {
      fprintf(stderr, "malloc error\n");
      exit(1);
    }

    tlist = buildList(n_arr, targ_count);

    //lets us start from different user specified target
    int start = 0;
    for (int x = 0; x < targ_count; x++) {
      if (strcmp(target, tlist[x]) == 0) {
        start = x;
      }
    }

    // Check if targets exist
    if (strcmp(target, "clean") != 0) {
      for (int x = 0; x < targ_count; x++) {
        checkTargets(n_arr, n_arr[x], tlist, targ_count);
      }
    }
    // If this is the target we want lets execute code
    checkDeps(tlist, n_arr[start], n_arr);

    for (int x = 0; x < targ_count; x++) {

      if (n_arr[x] -> target_name != NULL) free(n_arr[x] -> target_name);
      for (int j = 0; j < n_arr[x] -> dsize; j++) {
        if (n_arr[x] -> depends[j] != NULL) free(n_arr[x] -> depends[j]);
      }
      for (int y = 0; y < n_arr[x] -> csize; y++) {
        if (n_arr[x] -> commands[y] != NULL) free(n_arr[x] -> commands[y]);
      }
      if (n_arr[x] != NULL) free(n_arr[x]);
    }

    // Close File
    fclose(file);
  } else {
    fprintf(stderr, "Makefile not found\n");
    exit(1);
  }

  // Program finishes within expected bounds
  return 0;
}