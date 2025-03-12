//Xi Ting Yan wrote this program.
//Ali Kutay Dastan wrote the program that interfaces with Ollama. (The shell scripts included in this submission.)
//Tahmeed Ahmed helped with debugging and error handling.
//Ashkan Sharifi is working on improving the program is that pre-existing directories will be automatically indexed upon program start-up.
//This program takes user input to create directories and files with the specified titles and contents.
//The final goal of project Alpaca is to implement data streaming from Ollama into this program to create local files that store Ollama context.
//Ollama is an open-source software that allows local running of LLMs.

//These libraries include all the functions used by this program.
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "alpacaFunctions.h"

int main() {
    int whatDo;
    struct contextDir dirArr[MAX_DIR];
    for (int i = 0; i < MAX_DIR; i++) {
        dirArr[i].dirTitle[0] = 0;
    }

    struct dirent* dent;
    struct stat st;

    DIR *dir = opendir("./");
    if (dir == NULL) {
        printf("Error indexing directories.\n");
        return 1;
    }

    int dirIndex = 0;
    char prevDir[] = {'.', '.'};
    while ((dent = readdir(dir)) != NULL) {
        if (stat(dent->d_name, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                if (dent->d_name[0] != '.' && strcmp(dent->d_name, prevDir) != 0) {
                    strncpy(dirArr[dirIndex].dirTitle, dent->d_name, sizeof(dirArr[dirIndex].dirTitle) - 1);
                }
            }
        } else {
            printf("Error verifying pre-existing directories.\n");
        } 
        dirIndex++;
    }

    do {
        //Inquires the user what they wish to do.
        printf("Menu Options (Enter number to select option):\n1. Create Directory\n2. Create File\n0. Exit Program\nEnter your selection: ");
        scanf("%d", &whatDo);
        getchar();

        switch (whatDo) {
            case 1:
                createDir(dirArr);
                break;

            case 2:
                createFile(dirArr);
                break;

            case 0:
                printf("Exiting Program.\n");
                break;
            
            default:
                printf("Please enter a valid input according to the menu options.\n");
                break;
        }
    } while (whatDo != 0);

    return 0;
}