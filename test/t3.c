#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
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
  n->dsize = dsize;
  n->commands = commands;
  n->csize = csize;
  n->visits = 0;
  
  return n;
}

void runExec(int * final, struct node** d){

   int i = 0;
   //iterate through target nodes in order
   while(i < 2){ 
      printf("%d\n", final[i]);
      //iterate through that node's commands
      for(int j = 0; j < d[final[i]]->csize; j++){
         printf("%d\n", d[final[i]]->csize);
         char* argv[2];
         int x = 1;
         //seperate the command line into args
         char *buf;
         buf = strtok(d[final[i]]->commands[j], " "); //write getcchar func instead
         argv[0] = buf;
         while(1){
            printf("here\n");
            printf("coms:%s\n", d[final[i]]->commands[j]);
            buf = strtok(NULL, " ");
            argv[x] = buf;
            printf("arg%s\n", argv[x]);
            x++;
            if(x > 1) break;
         }
         
         //run that command
         int cstat;
         pid_t cpid = fork();
         
         if(cpid == 0){
            execvp(argv[0], argv);
            printf("execvp fail\n"); //shouldn't get here if execvp fails);
            exit(0);
         }else{
            pid_t t_pid = wait(&cstat);
            while(t_pid != cpid) t_pid = wait(&cstat);            
         }
         
    
         
      }
         
      i++;
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
  int final[2];
  final[0] = 0;
  final[1] = 1;
  
  runExec(final, narr);
  
  
}