/////////////////////////////////////////////////////////////
// CS 537, Fall 2020
// Program 3, 537make
// Michael Sachen, 9073631716, sachen
// Matt Jadin, 9065235468, jadin
/////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Take in a line as a char pointer, return 1 if it is a target line
int is_target(char * line) {
  // First char is tab or space exit

  if (isspace(line[0])) {
    return 0;
  }

  char tmp_line[strlen(line)];

  strcpy(tmp_line, line);
  char * token = strtok(tmp_line, ":");

  if (strcmp(token, line) == 0) {
    return 0;
  }

  return 1;
}

//removes white space from string
char * removeWhiteSpace(char * line) {
  char * rtn = malloc(4096 * sizeof(char));
  if (rtn == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  char t;
  int c = 0;
  for (int x = 0; x < strlen(line); x++) {
    t = line[x];

    if (t == EOF || t == '\n') break;
    if (t == ' ' || t == '\t') {

    } else if (t != ' ') {

      rtn[c] = t;

      c++;
    }
  }

  return rtn;
}

//splits lines by spaces
int splitter(char * line, char ** words, int lineNum) {

  int input_size = 4096;

  char t; //char buffer
  words[0] = malloc(input_size);
  if (words[0] == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  int j = 0;

  int c = 0;
  int z = 1; //keep track of multiple spaces
  for (int x = 0; x < strlen(line); x++) {

    t = line[x];

    if (t == EOF || t == '\n') break;
    if ((t == ' ' || t == '\t') && z == 0) {
      z = 1;
      j++;
      words[j] = malloc(input_size);
      if (words[j] == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(1);
      }
      c = 0;
    } else if (t != ' ' && t != '\t') {
      z = 0;
      words[j][c] = t;

      c++;
    }
  }
  if (c > 0) return j + 1;
  return j;

}