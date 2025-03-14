#include "dirStruct.h"

#define MAX_DIR 10
#define MAX_CONTENT_CHAR 5000

int indexDir(struct dirStruct dirArr[MAX_DIR]);
int listDir(struct dirStruct dirArr[MAX_DIR]);
void createDir(struct dirStruct dirArr[MAX_DIR]);
void createFile(struct dirStruct dirArr[MAX_DIR]);