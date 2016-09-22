#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "functions.c"

int main(){
    setlocale(LC_ALL, "Rus");
    printf("Type help to see manual\n");

    char *input = (char*)malloc(sizeof(char) * max_len_str);
    if (input == NULL){
        out_of_memory();
        return 0;
    }
    get_input(input);
    while (strcmp(input, ":q\n")){
        delete_last_symbol(input);
        if (!strcmp(input, "help")){
            write_help();
        } else if (is_com(input)){
            char **args = (char**)malloc(sizeof(char*) * max_num_args);   // arguments array
            char *in = (char*)malloc(sizeof(char) * max_len_str);
            char *out = (char*)malloc(sizeof(char) * max_len_str);
            int *it = (int*)malloc(sizeof(int) * max_num_args);
            in[0] = '\0', out[0] = '\0';
            if (args == NULL || in == NULL || out == NULL || it == NULL){
                out_of_memory();
                return 0;
            }
            split(input, args);
            int num_pipe = parse(args, in, out, it);
            if (num_pipe == 0){
                printf("Cant parse input\n");
                continue;
            }
            call_prog(args, in, out, num_pipe, it);
            freeArrayOfStrings(args);
            free(in);
            free(out);
            free(it);
        }
        get_input(input);
    }
    free(input);
    return 0;
}
