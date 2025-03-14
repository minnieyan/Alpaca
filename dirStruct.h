//This header file defines the struct that will represent directories and any relevant maximums.
#define MAX_TITLE_CHAR 20
#define MAX_FILES 50

struct dirStruct {
    char dirTitle[MAX_TITLE_CHAR + 1]; // + 1 to include null terminator.
    char fileTitle[MAX_FILES][MAX_TITLE_CHAR + 1]; // + 1 to include null terminator.
};