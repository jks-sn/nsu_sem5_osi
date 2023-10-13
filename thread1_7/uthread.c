#include "uhtread.h"
#include "uthread_scheduler.h"

uthread_t* uthread_init_main() {
    uthread_t* main_thread = (uthread_t*) malloc(sizeof(uthread_t) * 1);
    main_thread->uthread_context = (ucontext_t*) malloc(sizeof(ucontext_t) * 1);
    main_thread->uthread_id = 0;
    getcontext(main_thread->uthread_context); //run only from main
    sched_add_context(main_thread);
    printf("Start work\n");
    return main_thread;
}

int uthread_destroy_main(uthread_t* main_thread) {
    scheduler_clean();
    free(main_thread->uthread_context);
    free(main_thread);
    return SUCCESS;
}

void uthread_start_routine(void* arg) {
    uthread_t* thread = (uthread_t*) arg;
    thread->ret_val = thread->uthread_func(thread->uthread_args);
    thread->is_closed = TRUE;
    if (thread->is_detached == TRUE) {
        sched_delete_detached_context(thread);
    }
    else {
        while (TRUE) {
            sleep(1);
        }
    }
}

int uthread_create(uthread_t* thread, void*(start_routine), void* arg) {
    thread = (uthread_t*) malloc(sizeof(uthread_t));
    if(thread == NULL) {
        return 1;
    }
    thread->uthread_context = (ucontext_t*) malloc(sizeof(ucontext_t));
    if((*thread)->uthread_context == NULL) {
        return 1;
    }
    static int uthread_id = 1;
    thread->uthread_id = next_uthread_id++;
    thread->uthread_func = start_routine;
    thread->uthread_arg = arg;
    thread->ret_val = NULL;
    thread->is_joined = TRUE;
    thread->is_finished = FALSE;
    if (getcontext(thread->uthread_context) == -1)
        return 1;
    thread->uthread_context->uc_stack.ss_sp = mmap(NULL, stack_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK,-1, 0);
    if(thread->uthread_context->uc_stack.ss_sp == NULL) {
        return 1;
    }
    thread->uthread_context->uc_stack.ss_size = STACK_SIZE;
    makecontext(thread->uthread_context, (void (*)()) uthread_start_routine, 1, (void*) thread);
    sheduler_add(thread);
    printf("Thread %d created\n", thread->uthread_id);
    return 0;
}

int uthread_join(uthread_t* thread, void** retval) {
    if (thread->is_detached){
        return 1;
    }
    int thread_id = thread->uthread_id;
    while (TRUE) {
        if (thread->is_finished == 0) {
            sleep(1);
        }
        else {
            sheduler_delete(thread);
            if (thread->is_canceled){ 
                *retval = "thread was canceled\n"
            }
            else 
            {
                *retval = thread->ret_val;
            }
            munmap(thread->uthread_context->uc_stack.ss_sp);
            free(thread->uthread_context);
            free(thread);
            break;
        }
    }
    printf("Thread %d has been joined\n", thread_id);
    return SUCCESS;
}
