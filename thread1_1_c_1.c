#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#define THREAD_COUNT 5

void *mythread(void *arg) {
   printf("Thread ID: %ld, Process ID: %d, Parent Process ID: %d\n", pthread_self(), getpid(), getppid());
    return NULL;
}

int main() {
    pthread_t tids[THREAD_COUNT];
    int err;
    printf("Main Thread ID: %ld, Process ID: %d, Parent Process ID: %d\n", pthread_self(), getpid(), getppid());
    for (int i = 0; i < THREAD_COUNT; i++) {
        err = pthread_create(&tids[i], NULL, mythread, NULL);
	printf("Create ID: %ld \n", tids[i]);
        if (err) {
            printf("Main: pthread_create() failed: %s\n", strerror(err));
            return -1;
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
       err =  pthread_join(tids[i], NULL);
        if (err) {
            printf("Main: pthread_join() failed: %s\n", strerror(err));
            return -1;
        }
    }
    return 0;
}
