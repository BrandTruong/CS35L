// Brandon Truong
#include <stdio.h>
#include <stdlib.h>

//Function declaration
int frobcmp(char const* a, char const* b);
int compare(const void* a, const void* b);
void testInput();
void testOutput();
void printObsfuscated(char ** arr, int arr_n);

int main(void) {
//Part 1
    char ** arr = (char**) malloc(sizeof(char*));
    char * string = (char*) malloc(sizeof(char));
    int arr_n = 0, string_n = 0;
//Part 2
    char current = getchar();
    testInput();
    //Get characters until reaches end of stdin
    while (current != EOF) {
        if (current == ' ') { 
        //Reached end of word, reallocate memory in array
            if (string_n == 0) {
                //Nothing inside, duplicate space means to next iteration
                current = getchar();
                continue;
            }
            //End of the word should be marked with space
            string[string_n] = ' ';

            //Place into array
            arr[arr_n] = string;
            arr = realloc(arr, (arr_n + 5) * sizeof(char*));
            arr_n++;
            //Error
            if (arr == NULL) {
                fprintf(stderr, "Reallocation of array failed\n");
            }
            //Point to something else before allocating new memory
            string = NULL;
            string = (char*) malloc(sizeof(char));
            //Error
            if (string == NULL) {
                fprintf(stderr, "New allocation of string failed\n");
            }
            //Reset for next iterations
            string_n = 0;
            current = getchar();
            testInput();
            continue;
        }
        string[string_n] = current;
        string = realloc(string, (string_n + 5) * sizeof(char));
        string_n++;
        //Error
        if (string == NULL) {
            fprintf(stderr, "Reallocation of string failed\n");
        }
        current = getchar();
        testInput();
        //Read last string
        if (current == EOF) {
            string[string_n] = ' ';
            arr[arr_n] = string;
            arr = realloc(arr, (arr_n + 5) * sizeof(char*));
            arr_n++;
        }
    }
    //End of File, no more to read
//Part 3
    //Sorts using compare, derived from frobcmp
    qsort(arr, arr_n, sizeof(char*), compare);
//Part 4
    //Prints based on sorted arr
    printObsfuscated(arr, arr_n);
//Part 5
    //Free up memory
    for (int i = 0; i < arr_n; i++) {
        free(arr[i]);
    }
    free(arr);
    free(string);
    //Set to null
    arr = NULL;
    string = NULL;
    //Success
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
  if (ferror(stdout) != 0) {
    fprintf(stderr, "Failed to print to stdout\n");
    exit(1);
  }
}

//Part 3 implementation
int frobcmp(char const* a, char const* b) {
// When both a and b have a next char, say c1 and c2 respectively,
// if (c1 ^ 42) < (c2 ^ 42) then a < b;
// if (c1 ^ 42) > (c2 ^ 42) then a > b;
// if (c1 ^ 42) == (c2 ^ 42) then move on to the next pair of char's
// If one string is the proper prefix of another, then the shorter string is less than the longer one
    for (;; a++, b++) {
        //Deobfuscate only one char at a time
        int unobfus_a = (int) *a ^ 42;
        int unobfus_b = (int) *b ^ 42;
        //Same string, both ended in space
        if(*a == ' ' && *b == ' ') {
            return 0;
        }
        //Logic:
        if (unobfus_a > unobfus_b || *b == ' ') {
            return 1;
        }
        else if (unobfus_a < unobfus_b || *a == ' ') {
            return -1;
        }
        else {
            //Equal to each other, continue with loop
            continue;
        }
    }
}
//For use with qsort
int compare(const void* a, const void* b) {
    return frobcmp(*((const char**) a), *((const char**) b));
}

//Part 4 implementation
void printObsfuscated(char ** arr, int arr_n) {
// Loop through 2D array printing out each character, with each word separated by a space
    for (int i = 0; i < arr_n; i++) {
        int j;
        for (j = 0; arr[i][j] != ' '; j++) {
            putchar(arr[i][j]);
            testOutput();
        }
        //Reached end of word, separate with space
        putchar(arr[i][j]);
        testOutput();
    }
}