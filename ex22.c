//Sagiv Antebi 318159282
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


#define SIZE 150
#define ENTER "\n"

void clearString(char buffer[SIZE]);

void getThePathsFromFile(const char *path, char pathDIR[SIZE], char pathInput[SIZE], char pathOutput[SIZE]);

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        printf("Not enough Arguments\n");
        exit(1);
    }
    char pathDIR[SIZE];
    char pathInput[SIZE];
    char pathOutput[SIZE];
    getThePathsFromFile(argv[1], pathDIR, pathInput, pathOutput);
    

}

void getThePathsFromFile(const char *path, char pathDIR[SIZE], char pathInput[SIZE], char pathOutput[SIZE]) {
    int fptr = open(path, O_RDONLY);
    if (fptr < 0) {
        perror("after open");
        exit(-1);
    }
    char buffer[SIZE * 3];
    //reading the path content
    ssize_t fBuffer = read(fptr, buffer, SIZE * 3);
    if (fBuffer < 0) {
        perror("read failed");
        exit(-1);
    }
    char *token = strtok(buffer, ENTER);
    while (token != NULL) {
        strcpy(pathDIR,token);
        token = strtok(NULL, ENTER);
        strcpy(pathInput,token);
        token = strtok(NULL, ENTER);
        strcpy(pathOutput,token);
        token = strtok(NULL, ENTER);
    }

}

void clearString(char buffer[SIZE]) {
    int h;
    for (h = 0; h < SIZE; ++h) {
        buffer[h] = '\0';
    }
}





