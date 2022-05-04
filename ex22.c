//Sagiv Antebi 318159282
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

#define SIZE 150
#define ENTER "\n"
#define SLASH "/"

void clearString(char buffer[SIZE]);

void getThePathsFromFile(const char *path, char pathDIR[SIZE], char pathInput[SIZE], char pathOutput[SIZE]);

int checkIfDir(char path[SIZE]);

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        printf("Not enough Arguments\n");
        exit(1);
    }
    char pathDIR[SIZE];
    char pathInput[SIZE];
    char pathOutput[SIZE];
    char inDir[SIZE];
    int fptr = open(argv[1], O_RDONLY);
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
        strcpy(pathDIR, token);
        token = strtok(NULL, ENTER);
        strcpy(pathInput, token);
        token = strtok(NULL, ENTER);
        strcpy(pathOutput, token);
        token = strtok(NULL, ENTER);
    }
    close(fptr);
    //do all the test to check if the given args are valid to use
    //check if it's a dir path
    if (checkIfDir(pathDIR) == 0) {
        perror("Not a valid directory\n");
        exit(-1);
    }
    //Start explore the dir
    DIR *dip;
    DIR *dip2;
    struct dirent *dit;
    struct dirent *dit2;
    int lenFile;
    if (((dip = opendir(pathDIR)) == NULL)) {
        perror("problem open dir\n");
        exit(-1);
    }
    printf("Directory stream is now open\n");
    while ((dit = readdir(dip)) != NULL) {
        strcpy(inDir, pathDIR);
        strcat(inDir, SLASH);
        strcat(inDir, dit->d_name);
        //checks if the new path is a dir
        if (checkIfDir(inDir) != 0) {
            if (((dip2 = opendir(inDir)) == NULL)) {
                perror("problem open dir\n");
                exit(-1);
            }
            while ((dit2 = readdir(dip2)) != NULL) {
                lenFile = strlen(dit2->d_name);
                //checks if the file is the c file
                if (dit2->d_name[lenFile - 1] == 'c') {
                    printf("\n%s\n",dit2->d_name);
                }

            }

            if (closedir(dip2) == -1) {
                perror("problem close dir\n");
                exit(-1);
            }
        }
    }

    if (closedir(dip) == -1) {
        perror("problem close dir\n");
        exit(-1);
    }

    printf("\nDirectory stream is now closed\n");

}

void clearString(char buffer[SIZE]) {
    int h;
    for (h = 0; h < SIZE; ++h) {
        buffer[h] = '\0';
    }
}

int checkIfDir(char path[SIZE]) {
    struct stat statBuf;
    if (stat(path, &statBuf) != 0)
        return 0;
    return S_ISDIR(statBuf.st_mode);
}






