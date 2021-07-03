/////////////////////////////////////////////////////////////
// CS 537, Fall 2020
// Program 3, 537make
// Michael Sachen, 9073631716, sachen
// Matt Jadin, 9065235468, jadin
/////////////////////////////////////////////////////////////

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

//node data structre to hold info about each target
struct node {
  char * target_name; //name of this target
  char ** depends; //list of dependencies
  char ** commands; //list of commands
  int dsize;
  int csize;
  int visits; //times visited for cycle detection

};

struct node * CreateNode() {
  struct node * n = (struct node * ) malloc(sizeof(struct node));
  if (n == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  n -> target_name = malloc(sizeof(char) * 4096);
  if (n -> target_name == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  n -> commands = malloc(sizeof(char * ) * 1000);
  if (n -> commands == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  n -> csize = 0;
  n -> visits = 0;

  return n;
}