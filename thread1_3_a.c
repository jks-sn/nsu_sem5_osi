#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

struct MyStruct {
    int int_field;
    char *str_field;
};

void *thread_function(void *arg) {
    struct MyStruct *data = (struct MyStruct *)arg;
    printf("Integer Field: %d\n", data->int_field);
    printf("String Field: %s\n", data->str_field);
    return NULL;
}

int main() {
    struct MyStruct data;
    data.int_field = 42;
    data.str_field = "Hello, World!";
    pthread_t tid;
    int err = pthread_create(&tid, NULL, thread_function, (void *)&data);
    if (err) {
	    printf("Main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
	err = pthread_join(tid, NULL);
	if(err) {
        printf("Main: pthread_join() failed: %s\n", strerror(err));
        return -1;
	}
    return 0;
}
