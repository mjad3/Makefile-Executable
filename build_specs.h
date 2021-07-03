#ifndef SPEX
#define SPEX

char** buildList(struct node **n, int size);
void checkTargets(struct node** d, struct node *n, char** targets, int tsize);
char ** runList(char** targetList, struct node *n, char** runList);
int targetNum(char* name, char** targetList);
void checkDeps(char** targetList, struct node *n, struct node** narr);


#endif
