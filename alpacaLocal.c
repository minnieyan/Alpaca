#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include "alpacaLocal.h"

#ifdef _WIN32
    #define MKDIR(path) mkdir(path)
#else
    #define MKDIR(path) mkdir(path, 0777)
#endif

int indexDir(struct dirStruct dirArr[MAX_DIR]) {
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
                    dirIndex++;
                }
            }
        } else {
            printf("Error verifying pre-existing directories.\n");
            return 1;
        } 
    }
    return 0;
}

void createDir(struct dirStruct dirArr[MAX_DIR]) {
    if (dirArr[MAX_DIR - 1].dirTitle[0] != '\0') {
        printf("You have reached the maximum amount of directories. (%d)\n", MAX_DIR);
    } else {
        for (int i = 0; i < MAX_DIR; i++) {
            if (dirArr[i].dirTitle[0] == '\0') {
                printf("Type in the name of the directories: ");
                fgets(dirArr[i].dirTitle, sizeof(dirArr[i].dirTitle), stdin);
                dirArr[i].dirTitle[strcspn(dirArr[i].dirTitle, "\n")] = '\0';
                if (MKDIR(dirArr[i].dirTitle) == 0) {
                    printf("Directory '%s' created successfully.\n", dirArr[i].dirTitle);
                    break;
                } else {
                    printf("Error creating directory, directory may already exist.\n");
                    dirArr[i].dirTitle[0] = 0;
                    break;
                }
            }
        }
    }
}

void listDir(struct dirStruct dirArr[MAX_DIR]) {
    for (int i = 0; i < MAX_DIR - 1; i++) {
        if (dirArr[i].dirTitle[0] != '\0') {
            printf("%s[%d]\n", dirArr[i].dirTitle, i + 1);
        }
    }
}

void createFile(struct dirStruct dirArr[MAX_DIR]) {
    listDir(dirArr);
    int selectDir;
    printf("Which of these directories does the file belong to?: ");
    scanf("%d", &selectDir);
    getchar();
    selectDir--;
    if (dirArr[selectDir].dirTitle[0] == '\0' || selectDir < 0 || selectDir > MAX_DIR - 1) {
        printf("Invalid directory index entered. Please only enter a directory listed.\n");
        return;
    } else {
        char fileContent[MAX_CONTENT_CHAR + 1];
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