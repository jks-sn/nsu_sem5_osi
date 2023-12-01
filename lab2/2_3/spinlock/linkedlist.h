#include <pthread.h>

#define MAX_STRING_LENGTH 100

typedef struct _Node {
    char value[MAX_STRING_LENGTH];
    struct _Node* next;
    pthread_spinlock_t spin;
} Node;

typedef struct _Storage {
    Node* first;
} Storage;

void initStorage(Storage* storage);
Node* createNode(const char* value);
void addToStorage(Storage* storage, const char* value);
void printStorage(Storage* storage);
void fillStorageRandomly(Storage* storage, int numElements);
void freeStorage(Storage* storage);