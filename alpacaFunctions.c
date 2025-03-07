#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include "contextStructs.h"

#define MAX_DIR 10
#define MAX_CONTXT_CHAR 5000

#ifdef _WIN32
    #define MKDIR(path) mkdir(path)
#else
    #define MKDIR(path) mkdir(path, 0777)
#endif

void createDir(struct contextDir dirArr[MAX_DIR]) {
    if (dirArr[MAX_DIR - 1].dirTitle[0] != '\0') {
        printf("You have reached the maximum amount of categories. (%d)\n", MAX_DIR);
    } else {
        for (int i = 0; i < MAX_DIR; i++) {
            if (dirArr[i].dirTitle[0] == '\0') {
                printf("Type in the name of the category: ");
                fgets(dirArr[i].dirTitle, sizeof(dirArr[i].dirTitle), stdin);
                dirArr[i].dirTitle[strcspn(dirArr[i].dirTitle, "\n")] = '\0';
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
}

void createFile(struct contextDir dirArr[MAX_DIR]) {
    int selectDir;
    for (int i = 0; i < MAX_DIR; i++) {
        if (dirArr[i].dirTitle[0] != '\0') {
            printf("%s[%d]\n", dirArr[i].dirTitle, i + 1);
        }
    }
    printf("Which of these categories does the context belong to?: ");
    scanf("%d", &selectDir);
    getchar();
    selectDir--;
    if (dirArr[selectDir].dirTitle[0] == '\0' || selectDir < 0 || selectDir > MAX_DIR - 1) {
        printf("Invalid category index entered. Please only enter a category listed.\n");
        return;
    } else {
        char fileContent[MAX_CONTXT_CHAR + 1];
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
        char filePath[2*MAX_TITLE_CHAR + 2];
        snprintf(filePath, sizeof(filePath), "%s/%s", dirArr[selectDir].dirTitle, dirArr[selectDir].fileTitle[fileIndex]);
        FILE *fptr;
        fptr = fopen(filePath, "w");
        if (fptr == NULL) {
            printf("Error creating file.\n");
            return;
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
        return;
    }
}