#ifndef REP
#define REP

struct node{
  char* target_name;  //name of this target
  char** depends;     //list of dependencies
  char** commands;    //list of commands
  int dsize;
  int csize;
  int visits;         //times visited for cycle detection
};

struct node* CreateNode();

#endif //REP