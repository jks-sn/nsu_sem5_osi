#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

sched_context_t* head = NULL;
sched_context_t* now = NULL;
sched_context_t* tail = NULL;
void sigalarm_handler(int signum) {
    if (signum == SIGALRM) {
        if (head == NULL && now == NULL) 
            return;
        scheduler_swap();
    }
}

void scheduler_init(unsigned int delay_seconds, unsigned int delay_microseconds) {
    signal(SIGALRM, sigalarm_handler);
    struct itimerval timer;
    timer.it_value.tv_sec = delay_seconds;
    timer.it_value.tv_usec = delay_microseconds;
    timer.it_interval.tv_sec = delay_seconds;
    timer.it_interval.tv_usec = delay_microseconds;
    setitimer(ITIMER_REAL, &timer, NULL);
}


void scheduler_add(uthread_t *thread) {
    sched_context_t* ctx =  (sched_context_t*)malloc(sizeof(sched_context_t));
    ctx->next_context = NULL;
    ctx->thread = thread;
    ctx->is_deleted = 0;
    if(head == NULL) {
        head = ctx;
        now = ctx;
        tail = ctx;
    }
    else {
        tail->next_context = ctx;
        tail = tail->next_context;
    }
}

int scheduler_delete(uthread_t *thread) {
    if(head == NULL) {
        return 1;
    }
    sched_context_t* check_prev = head;
    sched_context_t* check = head->next_context;
    while((check != NULL) && (now->thread != thread)) {
        check_prev = check;
        check = check->next_context;
    }
    if(check == NULL) {
        return 1;
    }
    check->is_deleted = 1;
    return 0;
}

void scheduler_clean() {
    struct itimerval timer;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);
    if(head == NULL)
        return;
    sched_context_t* context_to_free = head;
    sched_context_t* next_context_to_free = head->next_context;
    while(next_context_to_free != NULL) {
        free(context_to_free);
        context_to_free = next_context_to_free;
        next_context_to_free = next_context_to_free->next_context;
    }
    free(context_to_free);
    head = NULL;
    now = NULL;
    tail = NULL;
}

void scheduler_swap() {
    sched_context_t* prev; 
    if(head == NULL) {
        return;
    }
    if(tail == NULL) {
        tail = head;
    }
    // if(now == NULL) {
    //     now = tail->next_context;
    //     swapcontext(now->context; tail->context);
    // }
    if(now->is_deleted) {
        while(now->is_deleted) {
        }
    }
    if(now == tail) {
        prev = tail;
        now = head;
    }
    else {

    }
}