#include "myspinlock.h"

void initSpinLock(SpinLock *lock) {
    atomic_flag_clear(&lock->flag);
}

void lockSpinLock(SpinLock *lock) {
    while (atomic_flag_test_and_set_explicit(&lock->flag, memory_order_acquire)) {
        sched_yield();
    }
}

void unlockSpinLock(SpinLock *lock) {
    atomic_flag_clear_explicit(&lock->flag, memory_order_release);
}