#include <stdio.h>
#include <stdlib.h>
#include<process.h>
#include<errno.h>
#include<string.h>
#include<dos.h>
#include <locale.h>

const int max_len_str = 1024;       // maximum length of string
const int max_num_args = 1024;      // maximum number of arguments

void delete_last_symbol(char *str){ // erase last symbol in string
    str[strlen(str) - 1] = '\0';
    return;
}

int is_com(char *line){             // return 0 if string contain only spaces
    int i;
    for (i = 0; i < strlen(line); i++){
        if (line[i] != ' '){
            return 1;
        }
        i++;
    }
    return 0;
}

void get_input(char *input){       // read input line
    printf("shell>>> ");
    fgets(input, max_len_str, stdin);
    return;
}

int skip_spaces(char* str, int pos){  // return first non-space position
    int i = pos;
    while (i < strlen(str) && str[i] == ' '){
        i++;
    }
    return i;
}

void out_of_memory(){
    printf("Error: out of memory\n");
    return;
}

void parse(char *input, char* path, char **args){   // parse input line
    int cur_pos = skip_spaces(input, 0);
    int pos_path = 0;
    while (cur_pos < strlen(input) && input[cur_pos] != ' '){
        path[pos_path] = input[cur_pos];
        cur_pos++, pos_path++;
    }
    cur_pos = skip_spaces(input, cur_pos);
    args[0] = path;
    int num_args = 1;
    while (cur_pos < strlen(input)){                            // search arguments
        int pos_args = 0;
        args[num_args] = (char*)malloc(sizeof(char) * max_len_str);
        if (args[num_args] == NULL){
            out_of_memory();
            return;
        }
        while (cur_pos < strlen(input) && input[cur_pos] != ' '){
            args[num_args][pos_args] = input[cur_pos];
            cur_pos++, pos_args++;
        }
        cur_pos = skip_spaces(input, cur_pos);
        num_args++;
        if (num_args > max_num_args){
            out_of_memory();
            return;
        }
    }
    args[num_args] = NULL;
    return;
}

void call_prog(char *path, const char *const *args){
    execvp(path, args);
    perror("ERROR");
    return;
}

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
            char *path = (char*)malloc(sizeof(char) * max_len_str);       // path string
            char **args = (char**)malloc(sizeof(char*) * max_num_args);   // arguments array
            if (path == NULL || args == NULL){
                out_of_memory();
                return 0;
            }
            parse(input, path, args);
            call_prog(path, args);
        }
        get_input(input);
    }

    return 0;
}
