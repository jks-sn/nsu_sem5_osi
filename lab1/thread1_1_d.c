#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#define THREAD_COUNT 5
int number_constant = 10;

void *mythread(void *arg) {
    int n = 42;
    printf("Thread ID: %ld, n before changing: %d, number_constant before changing: %d\n", pthread_self(), n, number_constant);
    n += 5;
    number_constant += 5;
    printf("Thread ID: %ld, n after changing: %d, number_constant after changing: %d\n", pthread_self(), n, number_constant);
    return NULL;
}

int main() {
    pthread_t tids[THREAD_COUNT];
    int err;

    printf("Main Thread ID: %ld, Process ID: %d, Parent Process ID: %d\n", pthread_self(), getpid(), getppid());

    for (int i = 0; i < THREAD_COUNT; i++) {
        err = pthread_create(&tids[i], NULL, mythread, NULL);
        if (err) {
            printf("Main: pthread_create() failed: %s\n", strerror(err));
            return -1;
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
       err =  pthread_join(tids[i], NULL);
        if (err) {
            printf("Main: pthread_create() failed: %s\n", strerror(err));
            return -1;
        }
    }
    return 0;
}
