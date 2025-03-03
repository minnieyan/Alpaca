//Xi Ting Yan wrote this program.
//Ali Kutay Dastan wrote the program that interfaces with Ollama. It was not included in the submission because it is not yet implemented.
//Tahmeed Ahmed helped with debugging and error handling.
//Ashkan Sharifi is working on improving the program is that pre-existing directories will be automatically indexed upon program start-up.
//This program takes user input to create directories and files with the specified titles and contents.
//The final goal of project Alpaca is to implement data streaming from Ollama into this program to create local files that store Ollama context.
//Ollama is an open-source software that allows local running of LLMs.

//These libraries include all the functions used by this program.
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include "contextStructs.h"

//Checks the platform of the user, and defines a function to create directories depending on the user's platform.
#ifdef _WIN32
    #define MKDIR(path) mkdir(path)
#else
    #define MKDIR(path) mkdir(path, 0777)
#endif

//Defining limits of the program to prevent excessive RAM usage. See contextStructs.h for more limits.
#define MAX_DIR 10
#define MAX_CONTXT_CHAR 5000

int main() {
    int whatDo;
    struct contextDir dirArr[MAX_DIR];
    for (int i = 0; i < MAX_DIR; i++) { //Windows requires the array to be initiated, this is not needed for Linux/MacOS.
        dirArr[i].dirTitle[0] = 0;
    }

    do {
        //Inquires the user what they wish to do.
        printf("Enter 1 to create a category, enter 2 to create a file, enter 3 to exit program: ");
        scanf("%d", &whatDo);
        getchar(); //We will use getchar() to protect fgets() from buffer overflow.

        switch (whatDo) {
            case (1):
                //Checks that the user has not exceeded the maximum permitted amount of directories created. Does not permit further creation if so.
                //Attempts to create a directory with user-specified name, returns error message if failed.
                if (dirArr[MAX_DIR - 1].dirTitle[0] != '\0') {
                    printf("You have reached the maximum amount of categories. (%d)\n", MAX_DIR);
                    break;
                } else {
                    for (int i = 0; i < MAX_DIR; i++) {
                        if (dirArr[i].dirTitle[0] == '\0') { //Seeks the first available directory in the array by checking if a title already exists.
                            printf("Type in name of category: ");
                            fgets(dirArr[i].dirTitle, sizeof(dirArr[i].dirTitle), stdin);
                            dirArr[i].dirTitle[strcspn(dirArr[i].dirTitle, "\n")] = '\0'; //fgets() collects new lines. This will replace it with null.
                            if (MKDIR(dirArr[i].dirTitle) == 0) {
                                printf("Directory '%s' created successfully.\n", dirArr[i].dirTitle);
                                break;
                            } else {
                                printf("Error creating directory, directory may already exist.\n");
                                break;
                            }
                        }
                    }
                }
                break;

            case(2):
                //Indexes the created directories, and asks the user which directory they wish to create the file in.
                //Checks that the user specified directory is valid, returns error otherwise.
                //Attempts to create file with user-specified title and content.
                int selectDir;
                for (int i = 0; i < MAX_DIR; i++) {
                    if (dirArr[i].dirTitle[0] != '\0') {
                        printf("%s[%d]\n", dirArr[i].dirTitle, i + 1); //The array of directories is zero-indexed, we add 1 so it makes more sense to the user.
                    }
                }

                printf("Which of these categories does the context belong to?: ");
                scanf("%d", &selectDir);
                getchar();
                selectDir--; //The array of directories is zero-indexed, but the user input is not, this ensures the correct directory is used.
                if (dirArr[selectDir].dirTitle[0] == '\0' || selectDir < 0 || selectDir > MAX_DIR - 1) {
                    printf("Invalid category index entered. Please only enter a category listed.\n");
                    break;
                } else {
                    char fileContent[MAX_CONTXT_CHAR];
                    int fileIndex;
                    for (int i = 0; i < MAX_FILES; i++) {
                        if (dirArr[selectDir].fileTitle[i][0] == '\0') { //Seeks the first available file by checking if a title already exists, then indexes it.
                            fileIndex = i;
                            break;
                        }
                    }
                    printf("Type in name of text file: ");
                    fgets(dirArr[selectDir].fileTitle[fileIndex], sizeof(dirArr[selectDir].fileTitle[fileIndex]), stdin);
                    dirArr[selectDir].fileTitle[fileIndex][strcspn(dirArr[selectDir].fileTitle[fileIndex], "\n")] = '\0'; //Removes new line.

                    char filePath[MAX_TITLE_CHAR + MAX_TITLE_CHAR + 2];
                    snprintf(filePath, sizeof(filePath), "%s/%s", dirArr[selectDir].dirTitle, dirArr[selectDir].fileTitle[fileIndex]); //Creates path for file creation.
                    
                    //Creates file and writes in user specified input. Returns error if unable to.
                    FILE *fptr;
                    fptr = fopen(filePath, "w");
                    if (fptr == NULL) {
                        printf("Error creating file.\n");
                        break;
                    }
                    printf("Type in the contents of the file (Type in END to stop):\n");

                    while (1) {
                        fgets(fileContent, sizeof(fileContent), stdin);
                        if (strncmp(fileContent, "END", 3) == 0) {
                            break;
                        }
                        fprintf(fptr, "%s", fileContent);
                    }
                    
                    fclose(fptr);
                    printf("File '%s' created.\n", dirArr[selectDir].fileTitle[fileIndex]);
                }
                break;

            default:
                printf("Exiting Program.\n");
                break;
        }
    } while (whatDo == 1 || whatDo == 2);

    //By default, program will exist if user input is neither 1 nor 2.

    return 0;
}