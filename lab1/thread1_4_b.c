#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void *thread_function(void *arg) {
    int counter = 0;
    while (1) {
        counter++;
        pthread_testcancel();
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;
    int err;
    err = pthread_create(&thread, NULL, thread_function, NULL);
    if (err) {
	    printf("Main: pthread_create() failed: %s\n", strerror(err));
        return 1;
    }
    err = pthread_cancel(thread);
    if (err) {
	    printf("Main: pthread_cansel() failed: %s\n", strerror(err));
        return 1;
    }

    pthread_join(thread, NULL);

    printf("Thread has been canceled\n");

    return 0;
}
