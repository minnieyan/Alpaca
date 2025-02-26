#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include "contextStructs.h"

#ifdef _WIN32
    #include <windows.h>
    #define MKDIR(path) _mkdir(path)
#else
    #include <unistd.h>
    #define MKDIR(path) mkdir(path, 0777)
#endif

#define MAX_DIR 50
#define MAX_CONTXT_CHAR 5000

int main() {
    int whatDo;
    struct contextDir dirArr[MAX_DIR];

    do {
        printf("Enter 1 to create a category, enter 2 to create a file, enter 3 to exit program: ");
        scanf("%d", &whatDo);

        switch (whatDo) {
            case (1):
                if (dirArr[MAX_DIR - 1].dirTitle[0] != '\0') {
                    printf("You have reached the maximum amount of categories. (%d)\n", MAX_DIR);
                    break;
                } else {
                    for (int i = 0; i < MAX_DIR; i++) {
                        if (dirArr[i].dirTitle[0] == '\0') {
                            printf("Type in name of directory: ");
                            fgets(dirArr[i].dirTitle, sizeof(dirArr[i].dirTitle), stdin);
                            if (MKDIR(dirArr[i].dirTitle) == 0) {
                                printf("Directory '%s' created successfully.\n", dirArr[i].dirTitle);
                                break;
                            } else {
                                printf("Error creating directory, directory may already exist.\n");
                                return 1;
                            }
                        }
                    }
                }
                break;

            case(2):
                char fileName[MAX_TITLE_CHAR];
                char fileContent[MAX_CONTXT_CHAR];
                printf("Type in name of text file: ");
                scanf("%s", fileName);
                
                FILE *fptr;
                fptr = fopen(fileName, "w");
                if (fptr == NULL) {
                    printf("Error creating file.\n");
                    return 1;
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
                printf("File '%s' created.\n", fileName);
                break;

            default:
                printf("Exiting Program.\n");
                break;
        }
    } while (whatDo == 1 || whatDo == 2);

    return 0;
}