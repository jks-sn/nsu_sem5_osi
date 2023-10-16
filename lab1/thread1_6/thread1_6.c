#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mythread.h"
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>


void* mythread_func(void *arg) {
    for(int i = 0; i < 5; ++i) {
        printf("hello: %s\n",  (char*)arg);
        //sleep(1);
    }
    return NULL;
}

void* mythread_func1(void *arg) {
    printf("func: %d\n", getpid());
    while(1) {
        printf("HELLO: %s\n",  (char*)arg);
        sleep(1);
    }
}

void* mythread_func2(void *arg) {
    printf("Robot ready to fight!\n");
    printf("Or no, i am die :(\n");
    return NULL;
}

int main() {
    printf("MAIN: %d\n", getpid());
    while(1) {
    mythread_struct_t thread;
    //printf("%x\n", thread);
    mythread_attr_t attr;
    //mythread_set_thread_detached_no(&attr);
    mythread_set_thread_detached_yes(&attr);
    char* arg_value = "Human";
    int err = mythread_create(&thread, &attr, mythread_func2, (void*) arg_value);
    if (err != 0) {
        perror("mythread_create");
        return 1;
    }
    //sleep(1);
    //sleep(5);
    //printf("%x\n", thread);
    //mythread_cancel(&thread);
    //mythread_join(&thread, NULL);
    //printf("TUTUTU\n");
    }
    //sleep(10);
    //munmap(thread.stack_address, thread.stack_size);
    printf("Thread has been joined\n");
    return 0;
}
