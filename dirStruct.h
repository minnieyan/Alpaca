// This header file defines the struct that will represent directories and any relevant maximums.
// MAX_TITLE_CHAR defines the maximum amount of character permitted in the title of directories and files.
// MAX_FILES defines the maximum amount of files permitted in a single directory.
// Both can be changed to allow the program to accomodate more files per directory and longer titles. This would come at the cost of more memory usage.
#define MAX_TITLE_CHAR 20
#define MAX_FILES 50

// Definition of the dirStruct which will is used to represent directories.
// dirTitle represents the titles of each directory.
// fileTitle represents the title of each file in each directory.
struct dirStruct {
    char dirTitle[MAX_TITLE_CHAR + 1]; // + 1 to include null terminator.
    char fileTitle[MAX_FILES][MAX_TITLE_CHAR + 1]; // + 1 to include null terminator.
};