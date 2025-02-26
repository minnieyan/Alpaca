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

#define MAX_DIR 10
#define MAX_CONTXT_CHAR 5000

int main() {
    int whatDo;
    struct contextDir dirArr[MAX_DIR];

    do {
        printf("Enter 1 to create a category, enter 2 to create a file, enter 3 to exit program: ");
        scanf("%d", &whatDo);
        getchar();

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
                            dirArr[i].dirTitle[strcspn(dirArr[i].dirTitle, "\n")] = '\0';
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
                int selectDir;
                for (int i = 0; i < MAX_DIR; i++) {
                    if (dirArr[i].dirTitle[0] != '\0') {
                        printf("%s[%d]\n", dirArr[i].dirTitle, i + 1);
                    }
                }

                printf("Which of these categories does the context belong to?: ");
                scanf("%d", &selectDir);
                getchar();
                char fileContent[MAX_CONTXT_CHAR];
                int fileIndex;
                for (int i = 0; i < MAX_FILES; i++) {
                    if (dirArr[selectDir].fileTitle[i][0] == '\0') {
                        fileIndex = i;
                        break;
                    }
                }
                printf("Type in name of text file: ");
                fgets(dirArr[selectDir].fileTitle[fileIndex], sizeof(dirArr[selectDir].fileTitle[fileIndex]), stdin);
                dirArr[selectDir].fileTitle[fileIndex][strcspn(dirArr[selectDir].fileTitle[fileIndex], "\n")] = '\0';
                
                FILE *fptr;
                fptr = fopen(dirArr[selectDir].fileTitle[fileIndex], "w");
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
                printf("File '%s' created.\n", dirArr[selectDir].fileTitle[fileIndex]);
                break;

            default:
                printf("Exiting Program.\n");
                break;
        }
    } while (whatDo == 1 || whatDo == 2);

    return 0;
}