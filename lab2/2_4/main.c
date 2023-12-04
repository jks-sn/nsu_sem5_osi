#include "mymutex.h"
#include "myspinlock.h"
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/syscall.h>
#include <stdatomic.h>

SpinLock mySpin;

void* criticalSection(void* arg) {
    int threadId = *((int*)arg);
    lockSpinLock(&mySpin);

    printf("Thread %d is in the critical section.\n", threadId);

    unlockSpinLock(&mySpin);
    return NULL;
}

int main() {
    initSpinLock(&mySpin);
    int err;

    pthread_t thread1, thread2;
    int id1 = 1, id2 = 2;

    err = pthread_create(&thread1, NULL, criticalSection, (void*)&id1);
    if (err) {
	printf("main: pthread_create() failed: %s\n", strerror(err));
	return -1;
    }
    err = pthread_create(&thread2, NULL, criticalSection, (void*)&id2);
    if (err) {
	printf("main: pthread_create() failed: %s\n", strerror(err));
	return -1;
    }

    err = pthread_join(thread1, NULL);
    if(err) {
	printf("Main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_join(thread2, NULL);
     if(err) {
	printf("Main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }

    return 0;
}