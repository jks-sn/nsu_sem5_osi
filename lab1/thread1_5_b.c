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
void sigint_handler_2(int signo) {
    SIGQUIT_received = 1;
    printf("Поток 3: Получен сигнал SIGINT\n");
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
    pthread_kill(pthread_self(), SIGINT);
    pthread_exit(NULL);
}

void *thread_3_function(void *arg) {
    signal(SIGINT, sigint_handler_2);
    printf("Поток 3: Ожидание сигнала SIGINT...\n");
    pthread_kill(pthread_self(), SIGINT);
    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2, thread3;

    pthread_create(&thread1, NULL, thread_1_function, NULL);
    pthread_create(&thread2, NULL, thread_2_function, NULL);
    pthread_create(&thread3, NULL, thread_3_function, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    return 0;
}
