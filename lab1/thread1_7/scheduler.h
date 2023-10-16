#include <ucontext.h>
#include "uthread.h"

typedef struct sched_context {
    ucontext_t* context;
    int uthread_id;
    struct sched_context* next_context;
    uthread_t* thread;
    int is_deleted;
} sched_context_t;

void sigalarm_handler(int signum);
void scheduler_init(unsigned int delay_seconds, unsigned int delay_microseconds);
void scheduler_add(uthread_t *thread);
int scheduler_delete(uthread_t *thread);
void scheduler_clean();
void scheduler_swap();