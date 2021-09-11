#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int targetNum(char* name, char** targetList){
  int i = 0;
  while(targetList[i]){
    if(strcmp(targetList[i], name) == 0){
      return i;
      break;
    }
    i++;
  }
  return -1;
}

int main(){
  struct stat stats;
  
  int y = stat("t2.c", &stats);
  
  struct tm dt;
  dt = *(gmtime(&stats.st_mtime));
  printf("\nModified on: %d-%d-%d %d:%d:%d\n", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900, 
                                              dt.tm_hour, dt.tm_min, dt.tm_sec);
  //if(dt.st_mtime > dt.st_mtime) printf("yes\n");
  
  char** targetList = malloc(sizeof(char*) * 8);
  targetList[0] = "test1";
  targetList[1] = "test2";
  targetList[2] = "test3";
  
  char* name = "test3";
  
  int x = targetNum(name, targetList);
  printf("x: %d\n", x);
  return 0;
}