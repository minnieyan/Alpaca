// This file defines all the functions used by the main function that involve manipulation local directories/files.
// All these libraries are required for program functionality.
#include <stdio.h> // Required for printf(), scanf(), fgets() etc.
#include <sys/stat.h> // Required for mkdir() and stat(). Also defines structs used by stat().
#include <string.h> // Required for functions that manipulate strings such as strcmp(), strncpy(), strcspn() etc.
#include <dirent.h> // Contains struct definitions used for indexing existing directories.
#include "alpacaLocal.h" // Contains definition of dirStruct (struct representing directories) and defined maximums.

// The function to create a directory in Windows and UNIX based systems (MacOS and Linux) are different.
// This defines the MKDIR function differently depending on the user's platform.
// Checks if user is running Windows.
#ifdef _WIN32
    #define MKDIR(path) mkdir(path)
#else // Assumes UNIX based OS if not Windows.
    #define MKDIR(path) mkdir(path, 0777)
#endif

// This function indexes all directories that exist in the same directory as the program.
int indexDir(struct dirStruct dirArr[MAX_DIR]) {

    // dirent.h defines a struct called dirent that holds information relevant to files/directories.
    // The function will use dirent structs to hold the name of directories.
    // The function initializes a pointer for the struct 'dent'.
    struct dirent *dent;

    // stat.h defines a struct called stat that holds information relevant to files/directories.
    // The function will use stat structs to hold the type of file that is being specified (e.g. file type or directory type).
    // The function initializes the struct as 'st'.
    struct stat st;

    // Initializes a pointer 'dir' and defining it as the directory the program is in (./).
    DIR *dir = opendir("./");
    
    // The function prints an error message, returns 1, and terminates if the pointer for the local directory fails to initialize.
    if (dir == NULL) {
        printf("Error indexing directories.\n\n");
        return 1;
    }

    int dirIndex = 0;
    char prevDir[] = {'.', '.'}; // This would be the directory name of the preceding directory.

    // readdir() reads the information of all files/directories in the local directory (dir) and sets the pointer to that information.
    // The while loop allows the function to go through all files/directories within the local directory.
    // 'dent' now contains information about the file/directory currently being handled.
    // The loop terminates when no more files/directories exist that have not been indexed yet (== NULL).
    while ((dent = readdir(dir)) != NULL) {
        
        // stat() takes a file/directory and stores it in a stat struct.
        // The function stores the file/directory being pointed to by 'dent' defined by the directory title 'd_name' which is a member of the dirent struct.
        if (stat(dent->d_name, &st) == 0) {

            // The function then uses S_ISDIR() to check if the file/directory is a directory.
            // st_mode is a member of the stat struct that holds the type of the file/directory represented by struct 'st'.
            // S_ISDIR returns 1 if the file/directory is a directory according to st.st_mode.
            if (S_ISDIR(st.st_mode)) {

                // A directory will only be indexed if it is not the current directory or the preceding directory.
                // The function checks this by checking if the name of the directory is '.' (Local directory).
                // strcmp() returns 0 if the two strings inputted are identical, the title of the directory is compared to the previous directory '..'.
                if (dent->d_name[0] != '.' && strcmp(dent->d_name, prevDir) != 0) {
                    
                    // strncpy() copies the content of one string into another within a defined size.
                    // The directory title is copied into the directory struct title, indexing it.
                    // The maximum size of the string copied over is the size of the directory struct title.
                    // The function subtracts the size by 1 to preserve the null terminator.
                    strncpy(dirArr[dirIndex].dirTitle, dent->d_name, sizeof(dirArr[dirIndex].dirTitle) - 1);

                    // The function increments dirIndex to move to the next directory in the directory struct.
                    dirIndex++;
                }
            }

        // The function prints an error message and return 1 if there was an error with stat() that caused it to return non-0.
        // Otherwise the function returns 0 upon successful execution.
        } else {
            printf("Error verifying pre-existing directories.\n\n");
            return 1;
        } 
    }
    return 0;
}

// This function creates a new directory in the same directory as the program, with user input as the name.
void createDir(struct dirStruct dirArr[MAX_DIR]) {
    // Checks if the maximum amount of directories already exists.
    // Checks if the first character in the title of the final directory indexed is null.
    // If it is null, this means less than 10 directories exist, and the function can proceed, print an error message otherwise.
    if (dirArr[MAX_DIR - 1].dirTitle[0] != '\0') {
        printf("You have reached the maximum amount of directories. (%d)\n\n", MAX_DIR);

    } else {
        // Goes down the array of directory structs to find the first empty one.
        // An empty element in the array of directory structs would have NULL as the first character in the title.
        for (int i = 0; i < MAX_DIR; i++) {
            if (dirArr[i].dirTitle[0] == '\0') {

                // We prompt the user to input the name of the directory.
                printf("Type in the name of the directory: ");
                fgets(dirArr[i].dirTitle, sizeof(dirArr[i].dirTitle), stdin);

                // fgets() collects the new line character that will be inputted by the user as they hit ENTER.
                // strcspn() scans the string (the directory title) and returns the index of the new line character (\n).
                // That character in that index will then be replaced with NULL (\0).
                dirArr[i].dirTitle[strcspn(dirArr[i].dirTitle, "\n")] = '\0';

                // MKDIR() takes a path and creates a directory and returns 0 upon success.
                // We are creating the directory in the same directory as the program, so no path is needed, just the title of the directory.
                // Hence the argument inputted into MKDIR() is just the user inputted title of the directory.
                if (MKDIR(dirArr[i].dirTitle) == 0) {
                    printf("Directory '%s' created successfully.\n\n", dirArr[i].dirTitle);
                    break;

                // If MKDIR() returns anything but 0, it failed, print error message.
                // To prevent the failed directory from being indexed, we set the first character of the title to 0.
                } else {
                    printf("Error creating directory, directory may already exist.\n\n");
                    dirArr[i].dirTitle[0] = 0;
                    break;
                }
            }
        }
    }
}

// This function prints all the indexed directories. Note that it DOES NOT scan and re-indexes directories, but simply prints what is already indexed.
int listDir(struct dirStruct dirArr[MAX_DIR]) {
    int dirExist = 0;
    for (int i = 0; i < MAX_DIR; i++) {

        // This function will skip any directory titles that start with NULL, as this indicates the directory has not been created/indexed.
        if (dirArr[i].dirTitle[0] != '\0') {
            printf("%s[%d]\n", dirArr[i].dirTitle, i + 1); // i + 1 because while the array is 0-indexed, the user will start counting from 1, not 0.
            dirExist = 1;
        }
    }

    // The function will return 0 once it finishes printing all indexed directories. It will return 1 instead of no directories are indexed. (dirExist == 0)
    if (dirExist == 1) {
        return 0;
    } else {
        return 1;
    }
}

// This function creates a text file in the user-specified directory with the user-specified title and content.
void createFile(struct dirStruct dirArr[MAX_DIR]) {
    
    // The function first lists all indexed directories so that the user may decide which directory they wish the file to be created in.
    // If the function returns 1, this means no directories are indexed, and the function terminates with an error message.
    if (listDir(dirArr) != 0) {
        printf("No directories exist. Please create one before creating a file.\n\n");
        return;
    }

    // The user will specify which directory they wish the new file to be created in.
    int selectDir;
    printf("Which of these directories does the file belong to?: ");
    scanf("%d", &selectDir);
    getchar(); // getchar(); protects the fgets() later in this function from overflow.
    selectDir--; // We decrement selectDir because the user's specified index is 1-indexed, but the array in the program is 0-indexed.

    // User input is validated. If the user specified a non-existent directory, or a number outside the range of the possible directories
    // an error message is printed, and the function terminates.
    // A non-existent directory will have NULL as the first character in the title, which what this checks for.
    if (dirArr[selectDir].dirTitle[0] == '\0' || selectDir < 0 || selectDir > MAX_DIR) {
        printf("Invalid directory index entered. Please only enter a directory listed.\n\n");
        return;

    // An array is initialized to contain the content of the to-be-created file.
    } else {
        char fileContent[MAX_CONTENT_CHAR + 1]; // MAX_CONTENT_CHAR + 1 to include the null terminator of the string.
        int fileIndex;

        // An non-existent file is defined by having a title that begins with NULL.
        // Once the function has identified one, it will index it as the file to be created.
        for (int i = 0; i < MAX_FILES; i++) {
            if (dirArr[selectDir].fileTitle[i][0] == '\0') {
                fileIndex = i;
                break;
            }
        }

        // The user is prompted to enter the title of the file. The entered title is then stored as the title of the file.
        printf("Type in name of text file: ");
        fgets(dirArr[selectDir].fileTitle[fileIndex], sizeof(dirArr[selectDir].fileTitle[fileIndex]), stdin);

        // Scans for new line character and replaces it will NULL.
        // How this works and why it is necessary is explained in the comments for the createDir() function above.
        dirArr[selectDir].fileTitle[fileIndex][strcspn(dirArr[selectDir].fileTitle[fileIndex], "\n")] = '\0';

        // A string is initialized to hold the path of where the file will be created.
        // The size of the string is defined as 2*MAX_TITLE_CHAR + 2. The 2*MAX_TITLE CHAR allows for enough memory to store the largest possible directory name 
        // and the largest possible file name. The + 2 allocates memory of the null terminator and the '/' needed to specify the path.
        // snprintf() concatenates both titles and places a '/' between them to create the path, and stores it in filePath.
        char filePath[2*MAX_TITLE_CHAR + 2];
        snprintf(filePath, sizeof(filePath), "%s/%s", dirArr[selectDir].dirTitle, dirArr[selectDir].fileTitle[fileIndex]);
        
        // The pointer *fptr of data type FILE is a pointer of memory allocated for a file.
        // The fopen() function takes an input as a path and creates the file in that path. The second argument specifies what to do with the file.
        // In this case, "w" refers to "write", allowing for input into the file text. 
        FILE *fptr;
        fptr = fopen(filePath, "w");

        // If the pointer failed to initialize, the function prints an error message and terminates.
        if (fptr == NULL) {
            printf("Error creating file.\n\n");
            return;
        }

        // User is prompted to type in contents of the file.
        // User input is stored into the string initialized for storing the file content.
        printf("Type in the contents of the file (Type in END to stop):\n");
        while (1) {
            fgets(fileContent, sizeof(fileContent), stdin);

            // strncmp() compares two strings and returns 0 if the specified first few characters are identical.
            // The strings being compared are 'END' and the line the user has entered since the last new line character (fgets() stops after a new line character).
            // The third argument '3' specifies that the first 3 characters of these two strings will be compared.
            // Once the first 3 characters the user enters since the last new line character is 'END' followed by another new line, the while loop breaks.
            if (strncmp(fileContent, "END", 3) == 0) {
                break;
            }

            // This prints the string in fileContent into the file pointer fptr.
            fprintf(fptr, "%s", fileContent);
        }

        // This ends the 'write' of the file and prints a success message.
        fclose(fptr);
        printf("File '%s' created.\n\n", dirArr[selectDir].fileTitle[fileIndex]);
        return;
    }
}