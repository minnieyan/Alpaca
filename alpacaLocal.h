// This header file declares the functions that will be used in the main function, the definitions of these functions can be found in alpacaLocal.c.
// dirStruct.h defines the struct used to represent directories. It was included because the declared functions take the struct as an argument.
#include "dirStruct.h"

// MAX_DIR is the maximum amount of directories that can be handled by the program.
// MAX_CONTENT_CHAR is the maximum amount of characters permitted in a single text file.
// Both of these can be changed to allow the program to accomodate more directories and content at the cost of additional memory usage.
#define MAX_DIR 10
#define MAX_CONTENT_CHAR 5000

// Declarations of all functions. See alpacaLocal.c for definitions.
int indexDir(struct dirStruct dirArr[MAX_DIR]);
int listDir(struct dirStruct dirArr[MAX_DIR]);
void createDir(struct dirStruct dirArr[MAX_DIR]);
void createFile(struct dirStruct dirArr[MAX_DIR]);