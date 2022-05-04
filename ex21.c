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
#define SPACE " "
#define ENTER "\n"
#define LOWER_A "a"
#define LOWER_Z "z"

int checkIfLettersEqual(char a, char b);

void removeSpacesAndNewLines(char buffer[SIZE + 1]);

void makeAllUpperCase(char buffer[SIZE + 1]);

void setStringToCompare(char buffer[SIZE + 1]);

void clearString(char buffer[SIZE]);

int main(int argc, const char *argv[]) {
    int numToReturn = 2;
    char firstPathBuffer[SIZE + 1], secondPathBuffer[SIZE + 1];
    int fBuffer, sBuffer;
    if (argc != 3) {
        printf("Not enough Arguments");
        exit(1);
    }
    if (strcmp(argv[1], argv[2]) == 0) {
        return 1;
    }
    int fptr = open(argv[1], O_RDONLY);
    if (fptr < 0) {
        perror("after open");
        exit(-1);
    }
    int sptr = open(argv[2], O_RDONLY);
    if (sptr < 0) {
        perror("after open");
        exit(-1);
    }
    //reading the first path content
    fBuffer = read(fptr, firstPathBuffer, SIZE);
    if (fBuffer < 0) {
        perror("read failed");
        exit(-1);
    }
    //reading the second path content
    sBuffer = read(sptr, secondPathBuffer, SIZE);
    if (sBuffer < 0) {
        perror("read failed");
        exit(-1);
    }
    //Check if the text is short then do only those text
    if (sBuffer < SIZE && fBuffer < SIZE) {
        if (strcmp(firstPathBuffer, secondPathBuffer) == 0) {
            return 1;
        }
        setStringToCompare(firstPathBuffer);
        setStringToCompare(secondPathBuffer);
        printf("the string is: %s , and his lengt is: %d\n", firstPathBuffer, strlen(firstPathBuffer));
        printf("the string is: %s , and his lengt is: %d\n", secondPathBuffer, strlen(secondPathBuffer));
        if (strcmp(firstPathBuffer, secondPathBuffer) == 0) {
            return 3;
        }
        return 2;
    }
    printf("1\n");
    int s, f;
    //check if the strings are equal
    int identicalBool = 1;
    f = 0;
    s = 0;
    int fLength, sLength;
    while ((fBuffer == SIZE) && (sBuffer == SIZE)) {
        if ((strcmp(firstPathBuffer, secondPathBuffer) == 0) && (f == 0) && (s == 0)) {
            f = fLength;
            s = sLength;
        } else {
            if (f == 0) {
                setStringToCompare(firstPathBuffer);
                fLength = strlen(firstPathBuffer);
            }
            if (s == 0) {
                setStringToCompare(secondPathBuffer);
                sLength = strlen(secondPathBuffer);
            }
            identicalBool = 0;
            printf("fLength f = %d | sLength s =  %d\n",fLength,sLength);
            while (s != sLength && f != fLength) {
                printf("index f = %d | index s =  %d\t",f,s);
                printf("%c compare to %c\t",firstPathBuffer[f],secondPathBuffer[s]);
                if (firstPathBuffer[f] == secondPathBuffer[s]) {
                    printf("%c compare to %c\n",firstPathBuffer[f],secondPathBuffer[s]);
                    f++;
                    s++;
                    continue;
                }
                close(fptr);
                close(sptr);
                printf("2\n");
                return 2;
            }

        }
        if (f == fLength) {
            //reading the first path content
            clearString(firstPathBuffer);
            fBuffer = read(fptr, firstPathBuffer, SIZE);
            if (fBuffer < 0) {
                perror("read failed");
                exit(-1);
            }
            f = 0;
            printf("\n\n\n\t\tthe firstPathBuffer after read %s\n",firstPathBuffer);
        }
        if (s == sLength) {
            //reading the second path content
            clearString(secondPathBuffer);
            sBuffer = read(sptr, secondPathBuffer, SIZE);
            if (sBuffer < 0) {
                perror("read failed");
                exit(-1);
            }
            s = 0;
        }
    }
    printf("1\n");
    //fBuffer or sBuffer are smaller than SIZE
    //checks if they are identical
    if (identicalBool && (strcmp(firstPathBuffer, secondPathBuffer) == 0) && (f == 0) && (s == 0)) {
        close(fptr);
        close(sptr);
        printf("not here\n");
        return 1;
    } else if (fBuffer == 0 && sBuffer == 0 && identicalBool == 0) {
        printf("dont think here either\n");
        close(fptr);
        close(sptr);
        return 3;
    } else if ((fBuffer < SIZE) && (sBuffer < SIZE)) {
        if (f == 0) {
            setStringToCompare(firstPathBuffer);
            fLength = strlen(firstPathBuffer) ;
        }
        if (s == 0) {
            setStringToCompare(secondPathBuffer);
            sLength = strlen(secondPathBuffer) ;
        }
        identicalBool = 0;
        while (s != sLength && f != fLength) {
            if (firstPathBuffer[f] == secondPathBuffer[s]) {
                f++;
                s++;
                continue;
            }
            close(fptr);
            close(sptr);
            return 2;
        }
        close(fptr);
        close(sptr);
        return 3;
    } else if (fBuffer < SIZE) {
        printf("fyy the sBuffer is %d\n",sBuffer);
        printf("fBuffer < SIZE\n");
        if (f == 0) {
            setStringToCompare(firstPathBuffer);
            fLength = strlen(firstPathBuffer) ;
            printf("the firstPathBuffer is now %s\n",firstPathBuffer);
        }
        while (sBuffer != 0) {
            if (s == 0) {
                printf("it shrink it\n");
                setStringToCompare(secondPathBuffer);
                sLength = strlen(secondPathBuffer);
                printf("the string is now %s\n",secondPathBuffer);
            }
            printf("fLength f = %d | sLength s =  %d\n",fLength,sLength);
            while (s != sLength && f != fLength) {
                printf("index f = %d | index s =  %d\t",f,s);
                printf("%c compare to %c\t",firstPathBuffer[f],secondPathBuffer[s]);
                if (firstPathBuffer[f] == secondPathBuffer[s]) {
                    printf("%c compare to %c\n",firstPathBuffer[f],secondPathBuffer[s]);
                    f++;
                    s++;
                    continue;
                }
                printf("got here????????\n");
                close(fptr);
                close(sptr);
                return 2;
            }
            if (s == sLength) {
                //reading the second path content
                clearString(secondPathBuffer);
                sBuffer = read(sptr, secondPathBuffer, SIZE);
                if (sBuffer < 0) {
                    perror("read failed");
                    exit(-1);
                }
                s = 0;
            }
        }

    }else if (sBuffer < SIZE) {
        if (s == 0) {
            setStringToCompare(firstPathBuffer);
            fLength = strlen(firstPathBuffer) ;
        }
        if (s == 0) {
            setStringToCompare(secondPathBuffer);
            sLength = strlen(secondPathBuffer) - 1;
        }
        while (sBuffer != 0) {
            if (f == 0) {
                setStringToCompare(firstPathBuffer);
                fLength = strlen(firstPathBuffer) ;
            }
            while (s != sLength && f != fLength) {
                if (firstPathBuffer[f] == secondPathBuffer[s]) {
                    f++;
                    s++;
                    continue;
                }
                close(fptr);
                close(sptr);
                return 2;
            }
            if (f == fLength) {
                //reading the second path content
                clearString(firstPathBuffer);
                fBuffer = read(fptr, firstPathBuffer, SIZE);
                if (fBuffer < 0) {
                    perror("read failed");
                    exit(-1);
                }
                f = 0;
            }
        }

    }
    close(fptr);
    close(sptr);
    printf("got here?");
    return 3;
}


void makeAllUpperCase(char buffer[SIZE + 1]) {
    int i;
    for (i = 0; i < strlen(buffer); ++i) {
        if (buffer[i] >= 97 && buffer[i] <= 123) {
            buffer[i] -= 32;
        }
    }
}

void removeSpacesAndNewLines(char buffer[SIZE + 1]) {
    printf("the given buffer : %s \t ",buffer);
    char newBufferNoSpace[SIZE + 1];
    char returnBuffer[SIZE + 1];
    clearString(newBufferNoSpace);
    clearString(returnBuffer);
    char *token = strtok(buffer, SPACE);
    while (token != NULL) {
        strcat(newBufferNoSpace, token);
        token = strtok(NULL, SPACE);
    }
    char *token2 = strtok(newBufferNoSpace, ENTER);
    while (token2 != NULL) {
        strcat(returnBuffer, token2);
        token2 = strtok(NULL, ENTER);
    }
    strcpy(buffer, returnBuffer);
    printf("return : %s \n ",buffer);
}


void setStringToCompare(char buffer[SIZE + 1]) {
    removeSpacesAndNewLines(buffer);
    makeAllUpperCase(buffer);
}

void clearString(char buffer[SIZE]){
    int h;
    for (h = 0; h < SIZE ; ++h) {
        buffer[h] = '\0';
    }


}





