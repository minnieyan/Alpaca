#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alpacaOllama.h"

#ifdef _WIN32
    #define popen _popen
    #define pclose _pclose
#endif

int chatWithOllama(char modelName[MAX_MODEL_NAME], struct dirStruct dirArr[MAX_DIR]) {
    char input[MAX_INPUT];
    char command[MAX_COMMAND];
    char buffer[MAX_BUFFER];
    char lastResponse[MAX_RESPONSE];  // Store full response
    char contextLoad[MAX_RESPONSE];
    int contextStatus = 0;
    for (int i = 0; i < MAX_RESPONSE; i++) {
        lastResponse[i] = '\0';
    }  // Initialize empty

    printf("\n--- Chat with %s ---\n", modelName);
    char loadContext;
    printf("Would you like to load a context file? (Y/N): ");
    scanf("%c", &loadContext);
    if (loadContext == 'Y' || loadContext == 'y') {
        if (listDir(dirArr) != 0) {
            printf("No context categories exist.\n");
            return 1;
        }
        int dirSelect;
        printf("Select the category the context belongs to: ");
        scanf("%d", &dirSelect);
        dirSelect--;
        if (dirArr[dirSelect].dirTitle[0] == '\0' || dirSelect < 0 || dirSelect >= MAX_DIR) {
            printf("Invalid directory index entered. Please only enter a directory listed.\n");
            return 1;
        }
        if (listFile(dirArr, dirSelect) != 0) {
            printf("No files exist in this directory.\n");
            return 1;
        }
        int fileSelect;
        printf("Select the context file to load: ");
        scanf("%d", &fileSelect);
        fileSelect--;
        if (dirArr[dirSelect].fileTitle[fileSelect][0] == '\0' || fileSelect < 0 || fileSelect >= MAX_FILES) {
            printf("Invalid file index entered. Please only enter a directory listed.\n");
            return 1;
        }
        FILE *fptr = fopen(dirArr[dirSelect].fileTitle[fileSelect], "r");
        if (fptr == NULL) {
            printf("Error reading context file.\n");
            return 1;
        }
        fscanf(fptr, "%s", contextLoad);
        contextStatus = 1;
    }

    printf("Type 'save' to save the last response, or 'exit' to quit.\n");


    while (1) {
        if (contextStatus == 0) {
            printf("\nYou: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0; // Remove newline
            if (strcmp(input, "exit") == 0) {
                printf("Exiting chat...\n");
                break;
            }
        }

        if (contextStatus == 0) {
            snprintf(command, sizeof(command), "echo \"%s\" | ollama run %s", input, modelName);
        } else {
            snprintf(command, sizeof(command), "echo \"%s\" | ollama run %s", contextLoad, modelName);
        }
        FILE *fp = popen(command, "r");
        if (fp == NULL) {
            printf("Error communicating with Ollama.\n");
            return 1;
        }

        printf("Ollama: ");
        lastResponse[0] = '\0';  // Reset response storage

        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            printf("%s", buffer);
            strncat(lastResponse, buffer, sizeof(lastResponse) - strlen(lastResponse) - 1);  // Append response
        }
        pclose(fp);

        // Ask user if they want to save the response
        printf("\nType 'save' to record this response or press Enter to continue: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; // Remove newline
        
        if (strcmp(input, "save") == 0) {
            if (createFile(dirArr, lastResponse) != 0) {
                return 1;
            }
        }
    }
    return 0;
}
