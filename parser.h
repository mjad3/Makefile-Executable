#ifndef PARSER
#define PARSER


char**  split(char* line);
int is_target(char* line);
int is_line();
char* removeWhiteSpace(char* line);
int splitter(char* line, char** words, int lineNum);

#endif // PARSER
