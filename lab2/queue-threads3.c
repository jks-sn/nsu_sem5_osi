#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>

#include <pthread.h>
#include <sched.h>

#include "queue.h"

#define RED "\033[41m"
#define NOCOLOR "\033[0m"
#define QUEUE_SIZE 1000000
sem_t semaphore_1;
sem_t semaphore_2;
sem_t semaphore_3;

void set_cpu(int n) {
	int err;
	cpu_set_t cpuset;
	pthread_t tid = pthread_self();

	CPU_ZERO(&cpuset);
	CPU_SET(n, &cpuset);

	err = pthread_setaffinity_np(tid, sizeof(cpu_set_t), &cpuset);
	if (err) {
		printf("set_cpu: pthread_setaffinity failed for cpu %d\n", n);
		return;
	}

	printf("set_cpu: set cpu %d\n", n);
}

void *reader(void *arg) {
	int expected = 0;
	queue_t *q = (queue_t *)arg;
	printf("reader [%d %d %d]\n", getpid(), getppid(), gettid());

	set_cpu(0);

	while (1) {
		int val = -1;
		sem_post(&semaphore_2);
		sem_wait(&semaphore_1);
		sem_wait(&semaphore_3);
		int ok = queue_get(q, &val);
		sem_post(&semaphore_3);
		if (!ok)
			continue;

		if (expected != val)
			printf(RED"ERROR: get value is %d but expected - %d" NOCOLOR "\n", val, expected);

		expected = val + 1;
	}

	return NULL;
}

void *writer(void *arg) {
	int i = 0;
	queue_t *q = (queue_t *)arg;
	printf("writer [%d %d %d]\n", getpid(), getppid(), gettid());

	set_cpu(1);

	while (1) {
		//int random_number = rand() % 10;
		//if (random_number == 0){
		//	usleep(1);
		//}
		sem_post(&semaphore_1);
		sem_wait(&semaphore_2);
		sem_wait(&semaphore_3);
		int ok = queue_add(q, i);
		sem_post(&semaphore_3);

		if (!ok)
			continue;
		i++;
	}

	return NULL;
}

int main() {
	pthread_t tid_reader, tid_writer;
	queue_t *q;
	int err;

	printf("main [%d %d %d]\n", getpid(), getppid(), gettid());

	q = queue_init(QUEUE_SIZE);

	if((err = sem_init(&semaphore_1, 0, 1))) {
		printf("main: sem_init() failed: %s\n", strerror(err));
		return -1;
	}

	if((err = sem_init(&semaphore_2, 0, QUEUE_SIZE))) {
		printf("main: sem_init() failed: %s\n", strerror(err));
		return -1;
	}

	if((err = sem_init(&semaphore_3, 0, QUEUE_SIZE))) {
		printf("main: sem_init() failed: %s\n", strerror(err));
		return -1;
	}
	// for(int i = 0; i < QUEUE_SIZE; ++i) {
	// 	sem_post(&semaphore_2);
	// }

	if ((err = pthread_create(&tid_reader, NULL, reader, q))) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	if ((err = sched_yield())) {
		printf("main: sched_yield() failed %s\n", strerror(err));
	}

	if ((err = pthread_create(&tid_writer, NULL, writer, q))) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	if((err = pthread_join(tid_reader, NULL))) {
		printf("Main: pthread_join() failed: %s\n", strerror(err));
        return -1;
	}

	if((err = pthread_join(tid_writer, NULL))) {
		printf("Main: pthread_join() failed: %s\n", strerror(err));
        return -1;
	}

	pthread_exit(NULL);
	if((sem_destroy(&semaphore_1))) {
		printf("main: sem_destroy() failed: %s\n", strerror(err));
		return -1;
	}

	if((sem_destroy(&semaphore_2))) {
		printf("main: sem_destroy() failed: %s\n", strerror(err));
		return -1;
	}

	return 0;
}
