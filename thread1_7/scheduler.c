#include "scheduler.h"

sched_context_t* head = NULL;
sched_context_t* now = NULL;

void sigalarm_handler(int signum) {
    if (signum == SIGALRM) {
        if (head == NULL && now == NULL) return;
        sched_next();
    }
}

void scheduler_init(unsigned int delay){ // in seconds
    signal(SIGALRM, sigalarm_handler);
    alarm(delay);
    sched_push();
}


void sheduler_add(uthread_t *thread) {
    sched_context_t* ctx =  (sched_context_t*)malloc(sizeof(sched_context_t));
    ctx->next_context = NULL;
    ctx->thread = thread;
        ctx->is_deleted = 0;
    if(head == NULL) {
        head = ctx;
        now = ctx;
    }
    else {
        now->next_context = ctx;
        now = next_context; 
    }
}

void sheduler_delete(uthread_t *thread) {
    sched_context_t* prev = NULL;
    sched_context_t* now = head;
    if(head->thread == thread) {
        head = head->next_context;
        free(now);
        return;
    }
    while((now != NULL) && (now->thread == thread)) {
        prev = now;
        now = now->next_context;
    }
    if(now != NULL) {
        prev->next_context = now->next_context;
        free(now);
    }
}
