//Sagiv Antebi 318159282
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


#define SIZE 150
#define SPACE " "
#define ENTER "\n"
#define LOWER_A 97
#define LOWER_Z 123


void removeSpacesAndNewLines(char buffer[SIZE]);

void makeAllUpperCase(char buffer[SIZE]);

void setStringToCompare(char buffer[SIZE]);

void clearString(char buffer[SIZE]);

int main(int argc, const char *argv[]) {
    int numToReturn = 2;
    char firstPathBuffer[SIZE], secondPathBuffer[SIZE];
    int fBuffer, sBuffer;
    if (argc != 3) {
        printf("Not enough Arguments\n");
        exit(1);
    }
    if (strcmp(argv[1], argv[2]) == 0) {
        return 1;
    }
    int fptr = open(argv[1], O_RDONLY);
    if (fptr < 0) {
        perror("Error in: open");
        exit(-1);
    }
    int sptr = open(argv[2], O_RDONLY);
    if (sptr < 0) {
        perror("Error in: open");
        exit(-1);
    }
    //reading the first path content
    fBuffer = read(fptr, firstPathBuffer, SIZE);
    if (fBuffer < 0) {
        perror("Error in: read");
        exit(-1);
    }
    //reading the second path content
    sBuffer = read(sptr, secondPathBuffer, SIZE);
    if (sBuffer < 0) {
        perror("Error in: read");
        exit(-1);
    }
    //Check if the text is short then do only those text
    if (sBuffer < SIZE && fBuffer < SIZE) {
        if (strcmp(firstPathBuffer, secondPathBuffer) == 0) {
            close(fptr);
            close(sptr);
            return 1;
        }
        setStringToCompare(firstPathBuffer);
        setStringToCompare(secondPathBuffer);
        if (strcmp(firstPathBuffer, secondPathBuffer) == 0) {
            close(fptr);
            close(sptr);
            return 3;
        }
        close(fptr);
        close(sptr);
        return 2;
    }
    int s, f;
    //check if the strings are equal
    int identicalBool = 0;
    f = 0;
    s = 0;
    int fLength, sLength;
    while ((fBuffer == SIZE) && (sBuffer == SIZE)) {
        if ((strcmp(firstPathBuffer, secondPathBuffer) == 0) && (f == 0) && (s == 0)) {
            f = fLength;
            s = sLength;
            identicalBool = 1;
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

        }
        if (f == fLength) {
            //reading the first path content
            clearString(firstPathBuffer);
            fBuffer = read(fptr, firstPathBuffer, SIZE);
            if (fBuffer < 0) {
                perror("Error in: read");
                exit(-1);
            }
            f = 0;
        }
        if (s == sLength) {
            //reading the second path content
            clearString(secondPathBuffer);
            sBuffer = read(sptr, secondPathBuffer, SIZE);
            if (sBuffer < 0) {
                perror("Error in: read");
                exit(-1);
            }
            s = 0;
        }
    }
    //fBuffer or sBuffer are smaller than SIZE
    //checks if they are identical
    if (identicalBool && (strcmp(firstPathBuffer, secondPathBuffer) == 0) && (f == 0) && (s == 0)) {
        close(fptr);
        close(sptr);
        return 1;
    } else if (fBuffer == 0 && sBuffer == 0 && identicalBool == 0) {
        close(fptr);
        close(sptr);
        return 3;
    } else if ((fBuffer < SIZE) && (sBuffer < SIZE)) {
        if (f == 0) {
            setStringToCompare(firstPathBuffer);
            fLength = strlen(firstPathBuffer);
        }
        if (s == 0) {
            setStringToCompare(secondPathBuffer);
            sLength = strlen(secondPathBuffer);
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
        if (f == 0) {
            setStringToCompare(firstPathBuffer);
            fLength = strlen(firstPathBuffer);
        }
        while (sBuffer != 0) {
            if (s == 0) {
                setStringToCompare(secondPathBuffer);
                sLength = strlen(secondPathBuffer);
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
            if (f == fLength && s != sLength) {
                close(fptr);
                close(sptr);
                return 2;

            }
            if (f == fLength && s == sLength) {
                close(fptr);
                close(sptr);
                return 3;
            }
            if (s == sLength) {
                //reading the second path content
                clearString(secondPathBuffer);
                sBuffer = read(sptr, secondPathBuffer, SIZE);
                if (sBuffer < 0) {
                    perror("Error in: read");
                    exit(-1);
                }
                s = 0;
            }
        }

    } else if (sBuffer < SIZE) {
        if (s == 0) {
            setStringToCompare(secondPathBuffer);
            sLength = strlen(secondPathBuffer);
        }
        while (fBuffer != 0) {
            if (f == 0) {
                setStringToCompare(firstPathBuffer);
                fLength = strlen(firstPathBuffer);
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
            if (s == sLength && f != fLength) {
                close(fptr);
                close(sptr);
                return 2;

            }
            if (s == sLength && f == fLength) {
                close(fptr);
                close(sptr);
                return 3;
            }
            if (f == fLength) {
                //reading the second path content
                clearString(firstPathBuffer);
                fBuffer = read(fptr, firstPathBuffer, SIZE);
                if (sBuffer < 0) {
                    perror("Error in: read");
                    exit(-1);
                }
                f = 0;
            }
        }

    }
    close(fptr);
    close(sptr);
    return 3;
}


void makeAllUpperCase(char buffer[SIZE]) {
    int i;
    for (i = 0; i < strlen(buffer); ++i) {
        if (buffer[i] >= LOWER_A && buffer[i] <= LOWER_Z) {
            buffer[i] -= 32;
        }
    }
}

void removeSpacesAndNewLines(char buffer[SIZE]) {
    char newBufferNoSpace[SIZE];
    char returnBuffer[SIZE];
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
}


void setStringToCompare(char buffer[SIZE]) {
    removeSpacesAndNewLines(buffer);
    makeAllUpperCase(buffer);
}

void clearString(char buffer[SIZE]) {
    int h;
    for (h = 0; h < SIZE; ++h) {
        buffer[h] = '\0';
    }


}





