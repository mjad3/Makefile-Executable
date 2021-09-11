#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

struct node{
  char* target_name;  //name of this target
  char** depends;     //list of dependencies
  char** commands;    //list of commands
  int dsize;
  int csize;
  int visits;         //times visited for cycle detection
};

struct node* CreateNode(char *name, char** depends, int dsize, char** commands, int csize){
  struct node* n = (struct node*)malloc(sizeof(struct node));
  
  n->target_name = name;
  n->depends = depends;
  dsize = dsize;
  n->commands = commands;
  csize = csize;
  n->visits = 0;
  
  return n;
}

void updateCMD(char** d){
  
}
//build arr of all possible dependencies
char** buildList(struct node **n, int size){
  char** arr = malloc(1024 * sizeof(char**));
  
  for(int i = 0; i < size; i++){
    arr[i] = n[i]->target_name;
    
  }
     
  return arr;
}


void checkTargets(struct node** d, struct node *n, char** arr, int size){
  //reset visits somewhere
  n->visits++;
  if(n->visits > size){
    printf("Dependency loop found!\n");
    //exit with error
  }
  for(int i = 0; i < n->dsize; i++){
    for(int j = 0; j < size; j++){
      if(strcmp(n->depends[i], arr[j]) == 0){
        
        checkTargets(d, d[j], arr, size);
        
      }
    }
    
  }
  
}




int main(){
  struct node* test;
  char* name = malloc(10 * sizeof(char));
  name = "Testname";
  char** depends = malloc(5 * sizeof(char*));
  depends[0] = "dep1.o";
  depends[1] = "dep2.h";
  char** coms = malloc(5 * sizeof(char*));
  coms[0] = "gcc main.c";
  coms[1] = "gcc test.c";
  test = CreateNode(name, depends, 2, coms, 2);
  
  struct node* rest;
  char* name2 = malloc(10 * sizeof(char));
  name2 = "Testname2";
  char** depends2 = malloc(5 * sizeof(char*));
  depends2[0] = "dep3.o";
  depends2[1] = "dep4.h";
  char** coms2 = malloc(5 * sizeof(char*));
  coms2[0] = "gcc main2.c";
  coms2[1] = "gcc test2.c";
  rest = CreateNode(name2, depends2, 2, coms2, 2);
  
  struct node* narr[2];
  narr[0] = test;
  narr[1] = rest;
  char** arr = buildList(narr, 2);
  for(int i = 0; i < 2; i++){
    printf("%s\n", arr[i]);
  }
  int cstat;
  pid_t cpid = fork();
  char* argv[] = {"gcc", "-Wall", "testmain.c", NULL};
  //argv[1] = "gcc";
  //argv[0] = "testmain.c";
  if(cpid == 0){
    execvp(argv[0], argv);
    printf("execvp fail\n"); //shouldn't get here if execvp fails);
    exit(0);
  }else{
    pid_t t_pid = wait(&cstat);
    while(t_pid != cpid){
      t_pid = wait(&cstat);
    }
    
  }
      
  
  return 0;
}