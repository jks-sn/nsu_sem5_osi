#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void *mythread(void *arg) {
    printf("Thread ID: %lu\n", pthread_self());

    pthread_exit(NULL);
}

int main() {
    while (1) {
        pthread_t tid;
        int err = pthread_create(&tid, NULL, mythread, NULL);
        if (err) {
	    	printf("main: pthread_create() failed: ma%s\n", strerror(err));
		    return -1;
        }
	    err = pthread_join(tid, NULL);
	    if(err) {
            printf("Main: pthread_join() failed: %s\n", strerror(err));
            return -1;
	    }
    }
    return 0;
}
