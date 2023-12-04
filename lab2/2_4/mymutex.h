#include <stdatomic.h>

typedef struct {
    atomic_int atom;
} Mutex;

void initMutex(Mutex* mutex);
void lockMutex(Mutex* mutex);
void unlockMutex(Mutex* mutex);