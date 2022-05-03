//Sagiv Antebi
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


#define SIZE 150
#define SPACE " "
#define ENTER "\n"


void removeSpacesAndNewLines(char buffer[151]);

int main(int argc, const char *argv[]) {
    char firstPathBuffer[SIZE+1], secondPathBuffer[SIZE+1];
    int fBuffer,sBuffer;
    if (argc != 3) {
        printf("Not enough Arguments");
        exit(1);
    }
    if (strcmp(argv[1], argv[2]) == 0) {
        return 1;
    }
    int fptr = open(argv[1],O_RDONLY);
    if (fptr < 0)
    {
        perror("after open ");
        exit(-1);
    }
    int sptr = open( argv[2],O_RDONLY);
    if (sptr < 0)
    {
        perror("after open ");
        exit(-1);
    }
    //reading the first path content
    fBuffer= read(fptr,firstPathBuffer,SIZE);
    if (fBuffer < 0){
        perror("read failed");
        exit(-1);
    }
    printf("%s\n",firstPathBuffer);
    char firstPathBufferNoSpaces[SIZE + 1];
    removeSpacesAndNewLines(firstPathBuffer);
    printf("%s\n",firstPathBuffer);
    //reading the second path content
    sBuffer= read(sptr,secondPathBuffer,SIZE);
    if (sBuffer < 0){
        perror("read failed");
        exit(-1);
    }
    removeSpacesAndNewLines(secondPathBuffer);
    printf("\n%s\n\n",secondPathBuffer);



    close(fptr);
    close(sptr);
    return 0;
}

void removeSpacesAndNewLines(char buffer[151]) {
    char newBufferNoSpace[SIZE + 1];
    char returnBuffer[SIZE + 1];
    strcpy(newBufferNoSpace,"");
    strcpy(returnBuffer,"");
    char *token = strtok(buffer,SPACE);
    while(token != NULL){
        strcat(newBufferNoSpace,token);
        token = strtok(NULL,SPACE);
    }
    char *token2 = strtok(newBufferNoSpace,ENTER);
    while(token2 != NULL){
        strcat(returnBuffer,token2);
        token2 = strtok(NULL,ENTER);
    }
    strcpy(buffer,newBufferNoSpace);
}




