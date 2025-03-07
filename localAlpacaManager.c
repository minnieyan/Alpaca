//Xi Ting Yan wrote this program.
//Ali Kutay Dastan wrote the program that interfaces with Ollama. (The shell scripts included in this submission.)
//Tahmeed Ahmed helped with debugging and error handling.
//Ashkan Sharifi is working on improving the program is that pre-existing directories will be automatically indexed upon program start-up.
//This program takes user input to create directories and files with the specified titles and contents.
//The final goal of project Alpaca is to implement data streaming from Ollama into this program to create local files that store Ollama context.
//Ollama is an open-source software that allows local running of LLMs.

//These libraries include all the functions used by this program.
#include <stdio.h>
#include "alpacaFunctions.h"

int main() {
    int whatDo;
    struct contextDir dirArr[MAX_DIR];
    for (int i = 0; i < MAX_DIR; i++) {
        dirArr[i].dirTitle[0] = 0;
    }

    do {
        //Inquires the user what they wish to do.
        printf("Enter 1 to create a category, enter 2 to create a file, enter 3 to exit program: ");
        scanf("%d", &whatDo);
        getchar();

        switch (whatDo) {
            case (1):
                createDir(dirArr);
                break;

            case(2):
                createFile(dirArr);
                break;

            default:
                printf("Exiting Program.\n");
                break;
        }
    } while (whatDo == 1 || whatDo == 2);

    return 0;
}