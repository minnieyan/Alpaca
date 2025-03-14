//Xi Ting Yan wrote this program.
//Ali Kutay Dastan wrote the program that interfaces with Ollama. (The shell scripts included in this submission.)
//Tahmeed Ahmed helped with debugging and error handling.
//Ashkan Sharifi is working on improving the program is that pre-existing directories will be automatically indexed upon program start-up.
//This program takes user input to create directories and files with the specified titles and contents.
//The final goal of project Alpaca is to implement data streaming from Ollama into this program to create local files that store Ollama context.
//Ollama is an open-source software that allows local running of LLMs.

//These libraries include all the functions used by this program.
#include <stdio.h>
#include "alpacaLocal.h"

int main() {
    int whatDo;
    struct dirStruct dirArr[MAX_DIR];
    for (int i = 0; i < MAX_DIR; i++) {
        dirArr[i].dirTitle[0] = 0;
    }

    if (indexDir(dirArr) == 1) {
        return 1;
    }

    printf("Alpaca: Ollama Assistant\n\n");

    do {
        //Inquires the user what they wish to do.
        printf("Menu Options (Enter number to select option):\n1. Create Directory\n2. Create File\n3. List Directories\n0. Exit Program\nEnter your selection: ");
        scanf("%d", &whatDo);
        getchar();

        switch (whatDo) {
            case 1:
                createDir(dirArr);
                break;

            case 2:
                createFile(dirArr);
                break;

            case 3:
                if (listDir(dirArr) == 0) {
                    printf("End of directory list.\n\n");
                } else {
                    printf("No directories exist.\n\n");
                }
                break;

            case 0:
                printf("Exiting Program.\n\n");
                break;
            
            default:
                printf("Please enter a valid input according to the menu options.\n\n");
                break;
        }
    } while (whatDo != 0);

    return 0;
}