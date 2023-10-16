#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

void *thread_function(void *arg) {
    pthread_t tid_main = *((pthread_t *) arg);
    int err;
    if ((err = pthread_join(tid_main, NULL))) {
        printf("THREAD: pthread_join() failed: %s\n", strerror(err));
    }
    return NULL;
}

int main() {
    pthread_t thread;
    int err;
    pthread_t tid_main = pthread_self();
    if ((err = pthread_create(&thread, NULL, thread_function, &tid_main))) {
            printf("Main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
    if ((err = pthread_join(thread, NULL))) {
        printf("Main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }
    return 0;
}
