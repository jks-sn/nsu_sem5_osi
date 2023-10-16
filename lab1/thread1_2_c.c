#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

struct ThreadData {
    char str[20];
};

void *thread_function(void *arg) {
    struct ThreadData *data = (struct ThreadData *)arg;
    strncpy(data->str, "Hello world\0", sizeof(data->str));
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;
    int err;
    struct ThreadData data;
    if ((err = pthread_create(&thread, NULL, thread_function, (void *) (&data)))) {
            printf("Main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
    if ((err = pthread_join(thread, NULL))) {
        printf("Main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }
    printf("Get from thread: %s\n", data.str);
    return 0;
}
