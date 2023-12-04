#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <sched.h>

typedef struct {
    atomic_flag flag;
} SpinLock;

void initSpinLock(SpinLock *lock);

void lockSpinLock(SpinLock *lock);

void unlockSpinLock(SpinLock *lock);