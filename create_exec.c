/////////////////////////////////////////////////////////////
// CS 537, Fall 2020
// Program 3, 537make
// Michael Sachen, 9073631716, sachen
// Matt Jadin, 9065235468, jadin
/////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include "representation.h"
#include "parser.h"

/* Run commands in Node's command list */
void runExec(struct node * d) {

  //iterate through node's commands
  for (int j = 0; j < d -> csize; j++) {
    char ** argv = malloc((4096 * sizeof(char)) / 3);
    if (argv == NULL) {
      fprintf(stderr, "malloc error\n");
      exit(1);
    }
    //seperate the command line into args
    //while(isspace((unsigned char)*d->commands[j])) d->commands[j]++;
    int size = splitter(d -> commands[j], argv, 0);

    if (size < 2) {
      fprintf(stderr, "Not enough command arguments\n");
      exit(1);
    }

    //run that command
    int cstat;
    pid_t cpid = fork();
    if (cpid == -1) {
      fprintf(stderr, "fork fail\n");
      exit(1);
    }
    if (cpid == 0) {
      execvp(argv[0], argv);
      fprintf(stderr, "execvp fail\n"); //shouldn't get here if execvp doesnt fail
      exit(1);
    } else {
      pid_t t_pid = wait( & cstat);
      while (t_pid != cpid) t_pid = wait( & cstat);
      if (WIFEXITED(cstat) && WEXITSTATUS(cstat)) {
        fprintf(stderr, "execvp error\n"); //if we get a file not found error or something similar
        exit(1);
      }
    }

  }
}