#define _GNU_SOURCE
#include "mythread.h"
#include <unistd.h>
#include <string.h>
#include <sys/bitypes.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <signal.h>
#include <sched.h>
#include <stdio.h>

void* create_stack(long stack_size) {
    void *stack;
    stack = mmap(NULL, stack_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK,-1, 0);
    memset(stack, 0, stack_size);
    return stack;
}

int mythread_join(mythread_struct_t* mythread, void **retval) {
    while(!(mythread->is_detached) && !(mythread->is_finished) && !(mythread->is_canceled)) {
        sleep(1); 
    }
    //printf("tatata\n");
    //printf("%x\n", (*mythread));
    if(retval != NULL) {
        *retval = mythread->retval; 
    }
    mythread->is_joined = 1;
    sleep(1);
    return 0;
}

int mythread_create(mythread_struct_t *mythread, mythread_attr_t* attr, void* (*start_routine)(void *), void *arg) {
   // static int thread_num = 0;
    //thread_num ++;
    void* child_stack = create_stack(STACK_SIZE);
    mythread->is_detached = attr->is_make_detached;
    mythread->stack_size = STACK_SIZE;
    mythread->stack_address = child_stack;
    mythread->mythread_id.id = getpid();
    mythread->start_routine = start_routine;
    mythread->arg = arg;
    mythread->is_canceled = 0;
    mythread->is_joined = 0;    
    mythread->is_finished = 0;
    //printf("abacaba1\n");
    sleep(1);
    if(clone(mythread_routine, child_stack + STACK_SIZE, CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_THREAD | CLONE_SIGHAND, (void*) mythread) == -1) {
        return 1;
    }
    return 0;
}

int mythread_cancel(mythread_struct_t* mythread) {
    kill(mythread->mythread_id.id, SIGCHLD);
    mythread->is_canceled = 1;
    return 0;
}

int mythread_routine(void *arg) {
    //printf("abacaba2\n");

    mythread_struct_t *mythread  = (mythread_struct_t*)arg;
    mythread->retval = mythread->start_routine(mythread->arg); // do routine;
    // printf("abacaba3\n");
     sleep(1);
     mythread->is_finished = 1; 
     while(!mythread->is_joined && !mythread->is_detached) {
        //printf("lol\n");
         sleep(1);
     }
    // printf("abacaba4\n");
     //sleep(1);
    return 0;
}  

void mythread_set_thread_detached_yes(mythread_attr_t* attr) {
    attr->is_make_detached = 1;
}

void mythread_set_thread_detached_no(mythread_attr_t* attr) {
    attr->is_make_detached = 0;
}