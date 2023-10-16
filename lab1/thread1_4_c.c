#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

char *global_string = NULL;

void cleanup(void *arg) {
    if (arg != NULL) {
        printf("cleanup is going\n");
        free(arg);
    }
}

void *thread_function(void *arg) {
    global_string = (char *)malloc(sizeof(char) * 12);
    if (!global_string) {
        printf("Error of allocation memory for string");
        pthread_exit(NULL);
    }
    pthread_cleanup_push(cleanup, global_string);
    while (1) {
        if (global_string) {
            strcpy(global_string, "hello world");
            printf("%s\n", global_string);
        }
        sleep(1);
    }
    printf("abacba\n");
    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;
    int err = pthread_create(&thread, NULL, thread_function, NULL);
    if (err) {
        printf("Main: pthread_create() failed: %s\n", strerror(err));
        return 1;
    }
    sleep(5);
    err = pthread_cancel(thread);
    if (err) {
        printf("Main: pthread_cancel() failed: %s\n", strerror(err));
        return 1;
    }
    pthread_join(thread, NULL);
    printf("Thread has been canceled\n");
    return 0;
}
