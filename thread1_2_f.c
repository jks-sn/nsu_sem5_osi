#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void *thread_function(void *arg) {
    pthread_t tid = pthread_self();
    printf("Thread ID: %lu\n", tid);
    return NULL;
}

int main() {
    while (1) {
        pthread_t thread_id;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        int err = pthread_create(&thread_id, &attr, thread_function, NULL);
        if (err) {
	    	printf("main: pthread_create() failed: ma%s\n", strerror(err));
		    return -1;
        }
        pthread_attr_destroy(&attr);
    }

    return 0;
}
