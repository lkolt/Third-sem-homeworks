#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void delete_last_symbol(char *str);
int is_com(char *line);
void get_input(char *input);
int skip_spaces(char* str, int pos);
void out_of_memory();
void do_redirect_in(char *path);
void do_redirect_out(char *path);
void split(char *input, char **args);
int parse(char **args, char *in, char *out, int* it);
void do_iter_conv(int num_pipe, int pipe_in, int pipe_out, int num_iter, const char *const *args, int *it);
void call_prog(const char *const *args, char *in, char *out, int num_pipe, int *it);
void freeArrayOfStrings(char** arr);
