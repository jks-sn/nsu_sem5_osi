#include <ucontext.h>
#include "uthread.h"

typedef struct sched_context {
    ucontext_t* context;
    struct sched_context* next_context;
    uthread_t* thread;
    int is_deleted;
} sched_context_t;