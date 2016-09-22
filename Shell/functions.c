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
        args[num_args][pos_args] = '\0';
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

int parse(char **args, char *in, char *out, int *it){
    int i = 0;
    int num_pipe = 0;
    while (args[i] != NULL){
        it[num_pipe] = i;
        while (args[i] != NULL && strcmp(args[i], "<") && strcmp(args[i], ">") && strcmp(args[i], "|")){
            i++;
        }
        while (args[i] != NULL && strcmp(args[i], "|")){
            if (!strcmp(args[i], "<")){
                if (args[i + 1] == NULL){
                    return 0;
                }
                strcpy(in, args[i + 1]);
                i += 2;
            } else if (!strcmp(args[i], ">")){
                if (args[i + 1] == NULL){
                    return 0;
                }
                strcpy(out, args[i + 1]);
                i += 2;
            } else {
                return 0;
            }
            args[i - 2] = NULL;
            args[i - 1] = NULL;
        }
        num_pipe++;
        if (args[i] == NULL){
            return num_pipe;
        }
        args[i] = NULL;
        i++;
    }
    return num_pipe;
}

void do_iter_conv(int num_pipe, int pipe_in, int pipe_out, int num_iter, const char *const *args, int *it){
    pid_t cpid;
    int fd[2];
    fd[0] = -1;

    if (num_iter >= num_pipe){
        return;
    }
    if (num_iter == num_pipe - 1){
        fd[1] = pipe_out;
    } else {
        if (pipe(fd) == -1){
            printf("ERROR PIPE\n");
            _exit(EXIT_FAILURE);
        }
    }

    cpid = fork();

    if (cpid == -1){
        printf("ERROR FORK\n");
        _exit(EXIT_FAILURE);
    }

    if (cpid == 0){
        if (fd[0] != -1)
            close(fd[0]);
        dup2(pipe_in, 0);
        dup2(fd[1], 1);
        execvp(args[it[num_iter]], args + it[num_iter]);
        close(pipe_in);
        close(fd[1]);
        perror("Error");
        _exit(EXIT_SUCCESS);
    } else {
        close(fd[1]);
        wait(0);
        do_iter_conv(num_pipe, fd[0], pipe_out, num_iter + 1, args, it);
        close(fd[0]);
        _exit(EXIT_SUCCESS);
    }
    return;
}

void call_prog(const char *const *args, char *in, char *out, int num_pipe, int *it){
    int pipe_in = 0;
    int pipe_out = 1;
    if (strlen(in) != 0){
            pipe_in = open(in, O_RDONLY);
    }

    if (strlen(out) != 0){
        pipe_out = open(out, O_WRONLY | O_TRUNC | O_CREAT);
    }

    pid_t cpid = fork();
    if (cpid == -1){
        printf("ERROR FORK\n");
        return;
    }
    if (cpid == 0){
        do_iter_conv(num_pipe, pipe_in, pipe_out, 0, args, it);
    } else {
        wait(NULL);
    }
    return;
}

void freeArrayOfStrings(char** arr){
    size_t i = 0;
    while (arr[i] != NULL) {
        free(arr[i++]);
    }
    free(arr);
}

void write_help(){
    printf("Shell doesn't work with folders and files with cyrillic characters\n");
    printf("Type \":q\" to terminate the program\n");
    printf("Type \"prog arg1 arg2 ...\" to run program \"prog\" with arguments \"args\"\n");
    printf("Type \"< path\" to redirect in stream\n");
    printf("Type \"> path\" to redirect out stream\n");
    printf("Type \"cmd1 args | cmd2 args | cmd3 args | ....\" to use pipe\n");
}
