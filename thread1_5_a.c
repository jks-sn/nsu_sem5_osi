#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

int SIGQUIT_received = 0;
int SIGINT_received = 0;

void sigint_handler(int signo) {
    SIGINT_received = 1;
    printf("Поток 2: Получен сигнал SIGINT\n");
}

void *thread_1_function(void *arg) {
    sigset_t signal_set;
    printf("Поток 1: Начало работы\n");
    sigfillset(&signal_set);
    pthread_sigmask(SIG_BLOCK, &signal_set, NULL);

    while (!(SIGQUIT_received && SIGINT_received)) {
        sleep(1);
    }

    printf("Поток 1: Завершение работы\n");
    pthread_exit(NULL);
}

void *thread_2_function(void *arg) {
    signal(SIGINT, sigint_handler);
    printf("Поток 2: Ожидание сигнала SIGINT...\n");
    while(!SIGINT_received) {
        sleep(1);
    }
    pthread_exit(NULL);
}

void* thread_3_function(void* arg) {
    int sig;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGQUIT);
    printf("Поток 3: Ожидание сигнала SIGQUIT...\n");
    int result = sigwait(&set, &sig);
    if (result == 0) {
        printf("Поток 3: Получил сигнал SIGQUIT\n");
        SIGQUIT_received = 1;

    } else {
        perror("sigwait");
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2, thread3;

    pthread_create(&thread1, NULL, thread_1_function, NULL);
    pthread_create(&thread2, NULL, thread_2_function, NULL);
    pthread_create(&thread3, NULL, thread_3_function, NULL);
    sleep(1);
    pthread_kill(thread3, SIGINT);
    sleep(1);
    pthread_kill(thread3, SIGQUIT);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    return 0;
}
