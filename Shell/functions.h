#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <errno.h>
#include <string.h>
#include <dos.h>
#include <locale.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>

void delete_last_symbol(char *str);
int is_com(char *line);
void get_input(char *input);
int skip_spaces(char* str, int pos);
void out_of_memory();
void do_redirect_in(char *path);
void do_redirect_out(char *path);
void split(char *input, char **args);
int parse(char **args);
void call_prog(const char *const *args);
void freeArrayOfStrings(char** arr);
