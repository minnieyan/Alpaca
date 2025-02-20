#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #define MKDIR(path) _mkdir(path)
#else
    #include <unistd.h>
    #define MKDIR(path) mkdir(path, 0777)
#endif

#define MAXLENGTH 500

int main() {
    int whatDo;

    do {
        printf("Enter 1 to create a directory, enter 2 to create a file, enter 3 to exit program: ");
        scanf("%d", &whatDo);

        switch (whatDo) {
            case (1):
                char dirName[MAXLENGTH];
                printf("Type in name of directory: ");
                scanf("%s", &dirName);
                if (MKDIR(dirName) == 0) {
                    printf("Directory '%s' created.\n", dirName);
                } else {
                    printf("Error creating directory.\n");
                    return 1;
                }
                break;

            case(2):
                char fileName[MAXLENGTH];
                char fileContent[MAXLENGTH];
                printf("Type in name of text file: ");
                scanf("%s", &fileName);
                
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