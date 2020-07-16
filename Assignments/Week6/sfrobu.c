//705326387 Brandon Truong
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

//Function declaration
int frobcmp(char const* a, char const* b);
int compare(const void* a, const void* b);
void errOut(const char* error_msg);
//void testInput();
//void testOutput();
void printObsfuscated(char ** arr, int arr_n);

//0 for no flag given, 1 for flag
int f_flag = 0;

/*//Global values for stdin, stdout, stderr
int STDIN_FILENO = 0;
int STDOUT_FILENO = 1;
int STDERR_FILENO = 2;
*/

int main(int argc, char *argv[]) {
//Checks if f_flag was given and if too many arguments
    if (argc > 2) {
        errOut("Too many arguments\n");
    }
    if (argc == 2) {
        if (strcmp(argv[1], "-f" ) == 0 ) {
            f_flag = 1;
        } else {
            errOut("Invalid option\n");
        }
    }
    struct stat fileData; 
    if (fstat(STDIN_FILENO, &fileData) < 0) {
        errOut("fstat error"); 
    }
    //Check if regular file
    int size_n = 0;
    if(S_ISREG(fileData.st_mode)) {
        size_n = fileData.st_size; 
    } else {
        size_n = 8000;
    }
    char* fileBuffer = NULL;
    fileBuffer = (char*) malloc(size_n + 20);
    if (fileBuffer == NULL) {
        errOut("Allocation error\n");
    }
    int max_bytes = 0;
    int current_bytes;
    if(S_ISREG(fileData.st_mode)) {
        max_bytes = read(STDIN_FILENO, fileBuffer, size_n);
        if (max_bytes < 0) {
            errOut("Failed to read stdin");
        }
    } else {
        do {
            current_bytes = read(STDIN_FILENO, &fileBuffer[max_bytes], (size_n / 3) + 1);
            if (current_bytes < 0) {
                errOut("Failed to read stdin");
            } else {
                max_bytes += current_bytes;
                if(max_bytes * 2 >= size_n){
                    size_n *= 2;
                    char* temp = realloc(fileBuffer, size_n * sizeof(char));
                    fileBuffer = temp;
                }
            }
        } while (current_bytes > 0);
    }
//Part 1
    char ** arr = (char**) malloc(sizeof(char*));
    char * string = (char*) malloc(sizeof(char));
    int arr_n = 0, string_n = 0;
//Part 2
    int bytes_read = 0;
    char current = fileBuffer[bytes_read];
    //Get characters until reaches end of stdin
    while (bytes_read < max_bytes) {
        if (current == ' ') { 
        //Reached end of word, reallocate memory in array
            if (string_n == 0) {
                //Nothing inside, duplicate space means to next iteration
                bytes_read++;
                current = fileBuffer[bytes_read];
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
                errOut("Reallocation of array failed\n");
            }
            //Point to something else before allocating new memory
            string = NULL;
            string = (char*) malloc(sizeof(char));
            //Error
            if (string == NULL) {
                errOut("New allocation of string failed\n");
            }
            //Reset for next iterations
            string_n = 0;
            bytes_read++;
            current = fileBuffer[bytes_read];
            continue;
        }
        string[string_n] = current;
        string = realloc(string, (string_n + 5) * sizeof(char));
        string_n++;
        //Error
        if (string == NULL) {
            errOut("Reallocation of string failed\n");
        }
        bytes_read++;
        current = fileBuffer[bytes_read];
        //Read last string
        if (bytes_read >= max_bytes) {
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
    /*
    for (int i = 0; i < arr_n; i++) {
        free(arr[i]);
    }
    */
    free(arr);
    free(string);
	free(fileBuffer);
    //Set to null
    arr = NULL;
    string = NULL;
    //Success
    exit(0);
}

//Implementation of functions

/*
//Checks if input does not fail
void testInput() {
    char* error_msg = "Failed to print to stdin\n";
    int m_write = write(STDERR_FILENO, error_msg, strlen(error_msg));
    exit(1);
}
//Checks if output does not fail
void testOutput() {
    char* error_msg = "Failed to print to stdout\n";
    int m_write = write(STDERR_FILENO, error_msg, strlen(error_msg));
    exit(1);
}
*/

//Change testInput and testOutput to single func to stderr

//If called, errors out msg and exits
void errOut(const char* error_msg) {
    write(STDERR_FILENO, error_msg, strlen(error_msg));
    exit(1);
}
//Part 3 implementation
int frobcmp(char const* a, char const* b) {
// When both a and b have a next char, say c1 and c2 respectively,
// if (c1 ^ 42) < (c2 ^ 42) then a < b;
// if (c1 ^ 42) > (c2 ^ 42) then a > b;
// if (c1 ^ 42) == (c2 ^ 42) then move on to the next pair of char's
// If one string is the proper prefix of another, then the shorter string is less than the longer one
//NEW: add f flag to be case insensitive (change to compare in upper-case)
    for (;; a++, b++) {
        //Deobfuscate only one char at a time
        char unobfus_a = *a ^ 42;
        char unobfus_b = *b ^ 42;
        if(f_flag == 1) {
            unobfus_a = (toupper(unobfus_a));
            unobfus_b = (toupper(unobfus_b));
        }
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
            int output = write(STDOUT_FILENO, &arr[i][j], 1);
            if (output < 1) {
                errOut("Failed to write to stdout");
            }
        }
        //Reached end of word, separate with space
        int output = write(STDOUT_FILENO, &arr[i][j], 1);
        if (output < 1) {
        errOut("Failed to write to stdout");
        }
    }
}
