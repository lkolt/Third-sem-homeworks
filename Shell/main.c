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
            if (args == NULL){
                out_of_memory();
                return 0;
            }
            split(input, args);
            if (parse(args) != 0){
                printf("Cant parse input\n");
            }
            call_prog(args);
            freeArrayOfStrings(args);
        }
        get_input(input);
    }
    free(input);
    return 0;
}
