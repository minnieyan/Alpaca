// Xi Ting Yan wrote this program.
// Ali Kutay Dastan wrote the program that interfaces with Ollama. He also designed the repository that was used for this project.
// Tahmeed Ahmed is responsible for debugging and error handling.
// Ashkan Sharifi *PLACEHOLDER*
// This program takes user input to create directories and files with the specified titles and contents.
// Compile alpacaMain.c with alpacaLocal.c in the same directory as alpacaLocal.h and dirStruct.h.
// The final goal of project Alpaca is to implement data streaming from Ollama into this program to create local files that store Ollama context.
// Ollama is an open-source software that allows local running of LLMs.

// These libraries include all the functions used by this file.
#include <stdio.h> // Required for printf() and scanf().
#include "alpacaLocal.h" // Contains declaration for functions used and defined maximums.

// The main program uses functions defined in alpacaLocal.c.
int main() {
    // whatDo will be used to store user's request for what they wish to do.
    // An array called dirArr is initialized to contain the directory structs the program will use.
    int whatDo;
    struct dirStruct dirArr[MAX_DIR];
    // This fills the first character of each directory title with 0.
    // We do this to ensure the other functions work correctly.
    // Other functions will check if the first character in a directory title is 0 to determine if the directory exists.
    for (int i = 0; i < MAX_DIR; i++) {
        dirArr[i].dirTitle[0] = 0;
    }

    // This function indexes existing directories in the same directory as the program. See alpacaLocal.c.
    // If this function fails, an error message is printed and the program terminates. (See alpacaLocal.c for error message.)
    if (indexDir(dirArr) == 1) {
        return 1;
    }

    // Initialization message, will only print once upon successful initialization of program.
    printf("Alpaca: Ollama Assistant\n\n");

    do {
        // Inquires the user what they wish to do. User input will determine next steps.
        printf("Menu Options (Enter number to select option):\n1. Create Directory\n2. Create File\n3. List Directories\n0. Exit Program\nEnter your selection: ");
        scanf("%d", &whatDo);
        getchar(); // Some functions uses fgets(), this getchar(); is to protect from overflow into fgets().

        switch (whatDo) {
            case 1:
                // This function creates a directory, see alpacaLocal.c for definition and error messages.
                createDir(dirArr);
                break;

            case 2:
                // This function creates a file, see alpacaLocal.c for definition and error messages.
                createFile(dirArr);
                break;

            case 3:
                // This function prints all indexed directories, see alpacaLocal.c for definition.
                // Function return 0 after successful execution. Returns 1 if no indexed directories exist.
                if (listDir(dirArr) == 0) {
                    printf("End of directory list.\n\n");
                } else {
                    printf("No directories exist.\n\n");
                }
                break;

            case 0:
                // This is the only way the user can terminate the program gracefully.
                printf("Exiting Program.\n\n");
                break;
            
            default:
                // Validation of user input. If user inputs invalid input undefined by menu, this message will print.
                printf("Please enter a valid input according to the menu options.\n\n");
                break;
        }

    // Loop terminates upon user requesting to exit program.
    } while (whatDo != 0);

    return 0;
}