#define MAX_CONTXT_CHAR 5000
#define MAX_TITLE_CHAR 20
#define MAX_FILES 50

struct contextFile {
    char contextTitle[MAX_TITLE_CHAR];
    char contextChar[MAX_CONTXT_CHAR];
};

struct contextDir {
    char dirTitle[MAX_TITLE_CHAR];
    struct contextFile fileArr[MAX_FILES];
};