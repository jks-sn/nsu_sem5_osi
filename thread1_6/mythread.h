#ifndef MYTHREAD_H
#define MYTHREAD_H
#define PAGE 4096
#define STACK_SIZE PAGE * 8

typedef struct {
   int id;
} mythread_t; 

typedef struct {
    int is_make_detached;
} mythread_attr_t;

typedef struct {
    mythread_t mythread_id;
    void* stack_address;
    long stack_size;
    void* (*start_routine)(void *);
    void* arg;
    void* retval;
    volatile int is_joined;
    volatile int is_finished;
    volatile int is_detached;
    volatile int is_canceled;
} mythread_struct_t;

int mythread_routine(void *arg);
void* create_stack(long stack_size);
int mythread_join(mythread_struct_t* mythread, void **retval);
int mythread_create(mythread_struct_t *mythread, mythread_attr_t* attr, void* (*start_routine)(void *), void *arg);
int mythread_cancel(mythread_struct_t* mythread);
void mythread_set_thread_detached_yes(mythread_attr_t* attr);
void mythread_set_thread_detached_no(mythread_attr_t* attr);
#endif