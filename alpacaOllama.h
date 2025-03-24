#include "alpacaLocal.h"

#define MAX_MODEL_NAME 20
#define MAX_INPUT 256
#define MAX_COMMAND 4096
#define MAX_BUFFER 1024
#define MAX_RESPONSE 4096

int chatWithOllama(char modelName[MAX_MODEL_NAME], struct dirStruct dirArr[MAX_DIR]);