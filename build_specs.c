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
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "representation.h"
#include "create_exec.h"

/* Returns a list of targets for easy access */
char ** buildList(struct node ** n, int size) {
  char ** arr = malloc(1024 * sizeof(char * ));
  if (arr == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  for (int i = 0; i < size; i++) {
    arr[i] = n[i] -> target_name;
  }

  return arr;
}

/* Checks for cycles in dependencies graph */
void checkTargets(struct node ** d, struct node * n, char ** targets, int tsize) {
  n -> visits++;
  if (n -> visits > tsize + 1) {
    fprintf(stderr, "Dependency loop found!\n");
    exit(1);
  }

  // Recursively check targets for repeat values
  for (int i = 0; i < n -> dsize; i++) {
    for (int j = 0; j < tsize; j++) {
      if (strcmp(n -> depends[i], targets[j]) == 0) {

        checkTargets(d, d[j], targets, tsize);
      }
    }
  }
}

/* Finds the position of a string in target list */
int targetNum(char * name, char ** targetList) {
  int i = 0;
  while (targetList[i]) {

    if (strcmp(targetList[i], name) == 0) {
      return i;
      break;
    }
    i++;
  }

  return -1;
}

/* Find the last time a file was written */
time_t getFileTime(char * file) {
  struct stat stats;

  if (stat(file, & stats) == 0) return stats.st_mtime;
  return 0;
}

/* Compares File modification times to see if they need updates */
int newerFile(char * tname, char * dname) {
  time_t t = getFileTime(tname);
  time_t d = getFileTime(dname);
  time_t c = getFileTime(""); //gets min file time to check for files that don't exist
  if (d > t || c == d) return 1;

  return 0;
}

/* Returns ordered list of targets to make */
void checkDeps(char ** targetList, struct node * n, struct node ** narr) {
  //tracks if any dependancy is newer than target to determine if we need to build it
  int b = 0;
  if (n -> dsize == 0) b = 1;

  //for item in nodes dep list, check if it needs to be built
  for (int i = 0; i < n -> dsize; i++) {
    //if target, check that target's deps, else make sure it exists
    int tnum = targetNum(n -> depends[i], targetList);
    if (tnum != -1) {
      checkDeps(targetList, narr[tnum], narr);
    } else if (access(n -> depends[i], F_OK) == -1) {
      fprintf(stderr, "File Not Found!\n");
      exit(1);
    }

    //check if dependancy is newer
    if (newerFile(n -> target_name, n -> depends[i])) {
      b = 1;
    }
  }

  //if(b) deps are ready, run execvp
  if (b) {
    runExec(n);
  }
}