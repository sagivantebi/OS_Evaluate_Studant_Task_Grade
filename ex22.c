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
#define COMMA ","

void clearString(char buffer[SIZE]);

int checkIfDir(char path[SIZE]);

void checkCompResult(char comp[SIZE]);

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
    int statChild;
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
    //do all the test to check if the given args are valid to use
    //check if it's a dir path
    if (checkIfDir(pathDIR) == 0) {
        perror("Not a valid directory\n");
        exit(-1);
    }



    //create the csv file
    int writeToResult = open("results.csv",O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (writeToResult < 0) {
        perror("Error in: open");
        exit(-1);
    }
    close(writeToResult);



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
                        //need to search here for the ./b.out file
                        if ((childID = fork()) == -1) {
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
                            if (wait(&statChild) == -1)
                                perror("Error in: wait\n");
                            if ((childID = fork()) == -1) {
                                perror("Error in: fork\n");
                                exit(-1);
                            }
                                //The child
                            else if (childID == 0) {
                                int outComp;
                                outComp = open("outComp.txt", O_WRONLY | O_TRUNC | O_CREAT, 0644);
                                if (outComp < 0) {
                                    perror("Error in: open\n");
                                    exit(-1);
                                }
                                dup2(outComp, 1);
                                close(outComp);
                                char *arg3[SIZE] = {"./comp.out", "out.txt", pathOutput};
                                if (execvp(arg3[0], arg3) == -1) {
                                    perror("Error in: execvp\n");
                                    exit(-1);
                                }
                            }
                                //The father
                            else {
                                if (wait(&statChild) == -1)
                                    perror("Error in: wait\n");
                                printf("ok\n");
                                if ((childID = fork()) == -1) {
                                    perror("Error in: fork\n");
                                    exit(-1);
                                }
                                    //The child
                                else if (childID == 0) {
                                    int outComp;
                                    outComp = open("outCompResult.txt", O_WRONLY | O_TRUNC | O_CREAT, 0644);
                                    if (outComp < 0) {
                                        perror("Error in: open\n");
                                        exit(-1);
                                    }
                                    dup2(outComp, 1);
                                    close(outComp);
                                    char *arg4[SIZE] = {"echo","$?"};
                                    if (execvp(arg4[0], arg4) == -1) {
                                        perror("Error in: execvp\n");
                                        exit(-1);
                                    }
                                } else {
                                    if (wait(&statChild) == -1)
                                        perror("Error in: wait\n");
                                    int openCompRes = open("outCompResult.txt",O_RDONLY);
                                    if (openCompRes < 0) {
                                        perror("Error in: open");
                                        exit(-1);
                                    }
                                    char bufferComp[SIZE];
                                    int openCompResBuffer = read(openCompRes, bufferComp, SIZE);
                                    if (openCompResBuffer < 0) {
                                        perror("Error in: read");
                                        exit(-1);
                                    }
                                    close(openCompRes);
                                    checkCompResult(bufferComp);
                                    writeToResult = open("results.csv",O_WRONLY | O_APPEND);
                                    if (writeToResult < 0) {
                                        perror("Error in: open");
                                        exit(-1);
                                    }
                                    char resultToAdd[SIZE];
                                    strcpy(resultToAdd,dit->d_name);
                                    strcat(resultToAdd,bufferComp);
                                    int resultLen = strlen(resultToAdd);
                                    if(write(writeToResult,resultToAdd, resultLen)!=resultLen)
                                    {
                                        perror("Error in: write");
                                        close(writeToResult);
                                        exit(-1);
                                    }
                                    close(writeToResult);
                                    printf("ok\n");
                                }
                            }
                        }
                    }
                }
            }
            //the deer has no c files
            if (countCFiles == 0) {
                //NEED TO HANDLE IT!
                continue;
            }

            if (closedir(dip2) == -1) {
                perror("problem close dir\n");
                exit(-1);
            }
        }
        //frees the argvChild allocated

        free(argvChild[3]);
        argvChild[3] = NULL;
    }

    if (closedir(dip) == -1) {
        perror("problem close dir\n");
        exit(-1);
    }

    printf("\nDirectory stream is now closed\n");

}

void checkCompResult(char comp[SIZE]) {
    if(strcmp(comp,"1")==0)
        strcpy(comp,",100,EXCELLENT\n");
    if(strcmp(comp,"2")==0)
        strcpy(comp,",50,WRONG\n");
    strcpy(comp,",75,SIMILAR\n");
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
        perror("Error in:stat");
    }
    return S_ISDIR(statBuf.st_mode);
}






