#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#define THREAD_COUNT 10
int d = 10;

void *mythread(void *arg) {
    int a = 10;
    static int b = 20;
    const int c = 30;
    printf("Thread ID: %ld, Local variable address: %p, local static variable address: %p, local const variable address: %p, global variable address: %p\n", pthread_self(), &a, &b, &c, &d);
    return NULL;
}

int main() {
    pthread_t tids[THREAD_COUNT];
    int err;
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
