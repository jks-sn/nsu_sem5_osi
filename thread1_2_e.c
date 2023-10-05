#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
void *thread_function(void *arg) {
    pthread_t tid = pthread_self();
    printf("Thread ID: %lu\n", tid);
    pthread_detach(tid);
    return NULL;
}

int main() {
    while (1) {
        pthread_t thread_id;
        int err = pthread_create(&thread_id, NULL, thread_function, NULL);
        if (err) {
	    	printf("main: pthread_create() failed: ma%s\n", strerror(err));
		    return -1;
        }
        sleep(1);
        
    }
    return 0;
}
