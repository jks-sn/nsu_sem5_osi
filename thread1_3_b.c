#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

struct MyStruct {
    int int_field;
    char *str_field;
};

void *thread_function(void *arg) {
    struct MyStruct *data = (struct MyStruct *)arg;
    printf("Integer Field: %d\n", data->int_field);
    printf("String Field: %s\n", data->str_field);
    free(data);
    return NULL;
}

int main() {
    struct MyStruct *data = (struct MyStruct *)malloc(sizeof(struct MyStruct));
    if (data == NULL) {
        printf("Memory allocation failed\n");
        return -1;
    }
    data->int_field = 42;
    data->str_field = "Hello, World!";
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int err = pthread_create(&tid, &attr, thread_function, (void *)data);
    if (err) {
	    printf("Main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
    sleep(1);
    err = pthread_attr_destroy(&attr);
    if (err) {
	    printf("Main: pthread_attr_destroy() failed: %s\n", strerror(err));
        return -1;
    }
    return 0;
}
