// Brandon Truong
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void errOut(const char* error_msg);
void testInput();
void testOutput();

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
    char current = getchar();
    testInput();
    while (current != EOF) {
        for (int i = 0; i < max_i; i++){
            if (current == from[i]) {
                current = to[i];
            }
        }
        putchar(current);
        current = getchar();
    }
    exit(0);
}

//Implementation of functions

//Checks if input does not fail
void testInput() {
  if (ferror(stdin) != 0) {
    fprintf(stderr, "Failed to process stdin\n");
    exit(1);
  }
}
//Checks if output does not fail
void testOutput() {
  if (ferror(stdin) != 0) {
    fprintf(stderr, "Failed to print to stdout\n");
    exit(1);
  }
}
void errOut(const char* error_msg) {
    fprintf(stderr, error_msg);
    exit(1);
}
