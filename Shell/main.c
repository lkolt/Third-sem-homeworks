#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "functions.c"

int main(){
    setlocale(LC_ALL, "Rus");
    printf("Shell doesn't work with folders and files with cyrillic characters\n");
    printf("Type \":q\" to terminate the program\n");
    printf("Type \"prog arg1 arg2 ...\" to run program \"prog\" with arguments \"args\"\n");

    char *input = (char*)malloc(sizeof(char) * max_len_str);
    if (input == NULL){
        out_of_memory();
        return 0;
    }
    get_input(input);
    while (strcmp(input, ":q\n")){
        delete_last_symbol(input);
        if (is_com(input)){
            char **args = (char**)malloc(sizeof(char*) * max_num_args);   // arguments array
            char *in = (char*)malloc(sizeof(char) * max_len_str);
            char *out = (char*)malloc(sizeof(char) * max_len_str);
            in[0] = '\0', out[0] = '\0';
            if (args == NULL || in == NULL || out == NULL){
                out_of_memory();
                return 0;
            }
            split(input, args);
            if (parse(args, in, out) != 0){
                printf("Cant parse input\n");
                continue;
            }
            call_prog(args, in, out);
            freeArrayOfStrings(args);
            free(in);
            free(out);
        }
        get_input(input);
    }
    free(input);
    return 0;
}
