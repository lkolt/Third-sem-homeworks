#include "functions.h"

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

void do_redirect_in(char *path){
    int in;
    in = open(path, O_RDONLY);
    dup2(in, 0);
    close(in);
    return;
}

void do_redirect_out(char *path){
    int out;
    out = open(path, O_WRONLY | O_TRUNC | O_CREAT);
    dup2(out, 1);
    close(out);
    return;
}

void split(char *input, char **args){
    int cur_pos = skip_spaces(input, 0);
    int num_args = 0;
    while (cur_pos < strlen(input)){
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

int parse(char **args){
    int i = 0;
    while (args[i] != NULL && strcmp(args[i], "<") && strcmp(args[i], ">")){
        i++;
    }
    if (args[i] == NULL){
        return 0;
    }
    while (args[i] != NULL){
        if (!strcmp(args[i], "<")){
            if (args[i + 1] == NULL){
                return 1;
            }
            do_redirect_in(args[i + 1]);
            i += 2;
        } else if (!strcmp(args[i], ">")){
            if (args[i + 1] == NULL){
                return 1;
            }
            do_redirect_out(args[i + 1]);
            i += 2;
        } else {
            return 1;
        }
        args[i - 1] = NULL;
        args[i - 2] = NULL;
    }
    return 0;
}

void call_prog(const char *const *args){
    execvp(args[0], args);
    perror("ERROR");
    return;
}

void freeArrayOfStrings(char** arr){
    size_t i = 0;
    while (arr[i] != NULL) {
        free(arr[i++]);
    }
    free(arr);
}
