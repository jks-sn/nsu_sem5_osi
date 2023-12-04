#include "mymutex.h"
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>

void initMutex(Mutex* mutex) {
    atomic_store(&mutex->atom, 0);
}

void lockMutex(Mutex* mutex) {
    int c = atomic_exchange(&mutex->atom, 1);
    if (c != 0) {
        do {
            if (c == 2 || atomic_exchange(&mutex->atom, 2) != 0) {
                syscall(SYS_futex, &mutex->atom, FUTEX_WAIT, 2, 0, 0, 0);
            }
        } while ((c = atomic_exchange(&mutex->atom, 2)) != 0);
    }
}

void unlockMutex(Mutex* mutex) {
    if (atomic_fetch_sub(&mutex->atom, 1) != 1) {
        atomic_store(&mutex->atom, 0);
        syscall(SYS_futex, &mutex->atom, FUTEX_WAKE, 1, 0, 0, 0);
    }
}