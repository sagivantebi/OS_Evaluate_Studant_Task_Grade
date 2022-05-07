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
#include <time.h>

#define SIZE 150
#define ENTER "\n"
#define SLASH "/"
#define DOT "."
#define TDOT ".."

void clearString(char buffer[SIZE]);

int checkIfDir(char path[SIZE]);

void checkCompResult(char comp[SIZE], int childResult);

void checkIfPathsAreValid(char dir[SIZE], char input[SIZE], char output[SIZE]);

void createCsvAndError();

void writeToResultCSV(char resultToAdd[SIZE]);

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        printf("Not enough Arguments\n");
        exit(1);
    }
    pid_t childID;
    char *argvChild[SIZE] = {"gcc", "-o", "b.out"};
    char pathDIR[SIZE];
    char pathInput[SIZE];
    char pathOutput[SIZE];
    char inDir[SIZE];
    char resultToAdd[SIZE];
    int statChild;
    int resultLen;
    int writeToResult;
    time_t start_t, end_t, total_t;
    int fptr = open(argv[1], O_RDONLY);
    if (fptr < 0) {
        perror("Error in: open\n");
        exit(-1);
    }
    char buffer[SIZE * 3];
    //reading the path content
    ssize_t fBuffer = read(fptr, buffer, SIZE * 3);
    if (fBuffer < 0) {
        perror("Error in: read\n");
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

    checkIfPathsAreValid(pathDIR, pathInput, pathOutput);
    createCsvAndError();

    //Start explore the dir
    DIR *dip;
    DIR *dip2;
    struct dirent *dit;
    struct dirent *dit2;
    int lenFile;
    if (((dip = opendir(pathDIR)) == NULL)) {
        perror("Error in: opendir\n");
        exit(-1);
    }
    int countCFiles;
    while ((dit = readdir(dip)) != NULL) {
        strcpy(inDir, pathDIR);
        strcat(inDir, SLASH);
        strcat(inDir, dit->d_name);
        //checks if the new path is a dir
        if (checkIfDir(inDir) != 0) {
            if (((dip2 = opendir(inDir)) == NULL)) {
                perror("Error in: opendir\n");
            }
            argvChild[3] = (char *) malloc(SIZE);
            //need to check if malloc failed
            countCFiles = 0;
            while ((dit2 = readdir(dip2)) != NULL) {
                lenFile = strlen(dit2->d_name);
                //checks if the file is the c file
                if (dit2->d_name[lenFile - 1] == 'c') {
                    countCFiles++;
                    printf("\n%s\n", dit2->d_name);
                    //The fork got an error
                    if ((childID = fork()) == -1) {
                        perror("Error in: fork\n");
                    }
                        //The child
                    else if (childID == 0) {
                        clearString(argvChild[3]);
                        strcpy(argvChild[3], inDir);
                        strcat(argvChild[3], SLASH);
                        strcat(argvChild[3], dit2->d_name);
                        if (execvp(argvChild[0], argvChild) == -1) {
                            perror("Error in: execvp\n");
                            exit(-1);
                        }
                    }
                        //The father
                    else {
                        if (wait(&statChild) == -1)
                            perror("Error in: wait\n");
                        int childExitStat = WEXITSTATUS(statChild);
                        if (childExitStat == 1) {
                            clearString(resultToAdd);
                            strcpy(resultToAdd, dit->d_name);
                            strcat(resultToAdd, ",10,COMPILATION_ERROR\n");
                            writeToResultCSV(resultToAdd);
                        }else if ((childID = fork()) == -1) {
                            perror("Error in: fork\n");
                            exit(-1);
                        }
                            //The child
                        else if (childID == 0) {
                            int in, out;
                            // open input and output files
                            in = open(pathInput, O_RDONLY);
                            if (in < 0) {
                                perror("Error in: open\n");
                                exit(-1);
                            }
                            out = open("out.txt", O_WRONLY | O_TRUNC | O_CREAT, 0644);
                            if (out < 0) {
                                perror("Error in: open\n");
                                exit(-1);
                            }
                            // replace standard input with input file
                            dup2(in, 0);

                            // replace standard output with output file
                            dup2(out, 1);

                            // close unused file descriptors
                            close(in);
                            close(out);

                            char *arg2[SIZE] = {"./b.out"};

                            if (execvp(arg2[0], arg2) == -1) {
                                perror("Error in: execvp\n");
                                exit(-1);
                            }
                        }
                            //The father
                        else {
                            start_t = time(NULL);
                            if (wait(&statChild) == -1)
                                perror("Error in: wait\n");
                            end_t = time(NULL);
                            total_t = end_t - start_t;
                            if (total_t > 5) {
                                printf("more than 5 sec");
                                clearString(resultToAdd);
                                strcpy(resultToAdd, dit->d_name);
                                strcat(resultToAdd, ",20,TIMEOUT\n");
                                writeToResultCSV(resultToAdd);
                            } else if ((childID = fork()) == -1) {
                                perror("Error in: fork\n");
                                exit(-1);
                            }
                                //The child
                            else if (childID == 0) {
                                char *arg3[SIZE] = {"./comp.out", "out.txt", pathOutput};
                                if (execvp(arg3[0], arg3) == -1) {
                                    perror("Error in: execvp\n");
                                    exit(-1);
                                }
                            }
                                //The father
                            else {
                                int boolToAdd = 1;
                                clearString(resultToAdd);
                                strcpy(resultToAdd, dit->d_name);
                                if (wait(&statChild) == -1)
                                    perror("Error in: wait\n");
                                if (wait(&statChild) > 5) {
                                    strcat(resultToAdd, ",20,TIMEOUT\n");
                                    boolToAdd = 0;
                                }
                                int childResult = WEXITSTATUS(statChild);
                                printf("the childResult is:\t %d\n", childResult);
                                if (boolToAdd) {
                                    char bufferComp[SIZE];
                                    checkCompResult(bufferComp, childResult);
                                    strcat(resultToAdd, bufferComp);
                                }
                                writeToResultCSV(resultToAdd);
                            }
                        }
                    }
                }
            }
            //the deer has no c files

            if (countCFiles == 0) {
                if (strcmp(dit->d_name, DOT) != 0 && strcmp(dit->d_name, TDOT) != 0) {
                    clearString(resultToAdd);
                    strcpy(resultToAdd, dit->d_name);
                    strcat(resultToAdd, ",0,NO_C_FILE\n");
                    writeToResultCSV(resultToAdd);
                }
            }
            if (closedir(dip2) == -1) {
                perror("Error in: closedir");
                exit(-1);
            }
        }
        //frees the argvChild allocated

        free(argvChild[3]);
        argvChild[3] = NULL;
    }

    if (closedir(dip) == -1) {
        perror("Error in: closedir");
        exit(-1);
    }
    remove("b.out");
    remove("out.txt");

}

void writeToResultCSV(char resultToAdd[SIZE]) {
    int writeToResult = open("results.csv", O_WRONLY | O_APPEND);
    if (writeToResult < 0) {
        perror("Error in: open");
        exit(-1);
    }
    int resultLen = strlen(resultToAdd);
    if (write(writeToResult, resultToAdd, resultLen) != resultLen) {
        perror("Error in: write");
        close(writeToResult);
        exit(-1);
    }
    close(writeToResult);
}

void createCsvAndError() {
    //create the csv file
    int writeToResult = open("results.csv", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (writeToResult < 0) {
        perror("Error in: open");
        exit(-1);
    }
    close(writeToResult);

    //create the error.txt file
    int errorFile = open("error.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (errorFile < 0) {
        perror("Error in: open");
        exit(-1);
    }
    close(errorFile);
}

void checkIfPathsAreValid(char dir[150], char input[150], char output[150]) {
    //check if it's a dir path
    if (checkIfDir(dir) == 0) {
        perror("Not a valid directory\n");
        exit(-1);
    }
    int fptr = open(input, O_RDONLY);
    if (fptr < 0) {
        perror("Input file not exist\n");
        exit(-1);
    }
    char buffer[SIZE];
    //reading the path content
    ssize_t fBuffer = read(fptr, buffer, SIZE);
    if (fBuffer < 0) {
        perror("Input file not exist\n");
        exit(-1);
    }
    close(fptr);
    fptr = open(output, O_RDONLY);
    if (fptr < 0) {
        perror("Output file not exist\n");
        exit(-1);
    }
    char buffer1[SIZE];
    //reading the path content
    ssize_t fBuffer1 = read(fptr, buffer1, SIZE);
    if (fBuffer < 0) {
        perror("Output file not exist\n");
        exit(-1);
    }
    close(fptr);
}

void checkCompResult(char comp[SIZE], int childResult) {
    clearString(comp);
    if (childResult == 1)
        strcpy(comp, ",100,EXCELLENT\n");
    else if (childResult == 2)
        strcpy(comp, ",50,WRONG\n");
    else
        strcpy(comp, ",75,SIMILAR\n");
}

void clearString(char buffer[SIZE]) {
    int h;
    for (h = 0; h < SIZE; ++h) {
        buffer[h] = '\0';
    }
}

int checkIfDir(char path[SIZE]) {
    struct stat statBuf;
    if (stat(path, &statBuf) == -1) {
        perror("Error in: stat");
    }
    return S_ISDIR(statBuf.st_mode);
}






