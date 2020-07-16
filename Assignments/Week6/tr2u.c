//705326387 Brandon Truong
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void errOut(const char* error_msg);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errOut("Not valid amount of args\n");
    }
    char* from = argv[1];
    char* to = argv[2];
    int max_i = strlen(from);
    //First error out, check if different lengths in from and to
    if (max_i != strlen(to)){
        errOut("To and from strings are not the same length\n");
    }
    //Second error out, check if duplicate in from
    for (int i = 0; i < max_i; i++) {
        char current = from[i];
        for (int j = i + 1; j < max_i; j++) {
            if (current == from[j]) {
                errOut("Duplicate char in from\n");
            }
        }
    }
    char current;
    while (read(STDIN_FILENO, &current, 1) > 0) {
        for (int i = 0; i < max_i; i++){
            if (current == from[i]) {
                current = to[i];
            }
        }
        write(STDOUT_FILENO, &current, 1);
        if(current == EOF) {
            break;
        }
    }
    exit(0);
}

//Implementation of functions

void errOut(const char* error_msg) {
    write(STDERR_FILENO, error_msg, strlen(error_msg));
    exit(1);
}