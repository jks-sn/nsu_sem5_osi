typedef struct {
    void* (*start_routine)(void *);
    void* uthread_args;
    void* retval;
    ucontext_t* uthread_context;
    int uthread_id;
    volatile int is_joined;
    volatile int is_detached;
    volatile int is_finished;
}uthread_t;

uthread_t* uthread_init_main();
int uthread_destroy_main(uthread_t* main_thread);
void uthread_start_routine(void* arg);
int uthread_create(uthread_t* thread, void*(start_routine), void* arg);
int uthread_join(uthread_t* thread, void** retval);