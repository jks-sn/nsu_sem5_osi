#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>
#include <sched.h>

#include "queue.h"

#define RED "\033[41m"
#define NOCOLOR "\033[0m"

static queue_t *q;

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
	pthread_spinlock_t *spin = (pthread_spinlock_t*)arg;
	printf("reader [%d %d %d]\n", getpid(), getppid(), gettid());

	set_cpu(1);

	while (1) {
		int val = -1;
		pthread_spin_lock(spin);
		int ok = queue_get(q, &val);
		pthread_spin_unlock(spin);
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
	pthread_spinlock_t *spin = (pthread_spinlock_t*)arg;
	printf("writer [%d %d %d]\n", getpid(), getppid(), gettid());

	set_cpu(1);
	srand(time(NULL));

	while (1) {
		//int random_number = rand() % 10;
		//if (random_number == 0){
		//	usleep(1);
		//}
		pthread_spin_lock(spin);
		int ok = queue_add(q, i);
		pthread_spin_unlock(spin);
		if (!ok)
			continue;
		i++;
	}

	return NULL;
}

int main() {
	pthread_t tid;
	int err;
	q = queue_init(1000000);


	printf("main [%d %d %d]\n", getpid(), getppid(), gettid());

	pthread_spinlock_t spin;
 
    	pthread_spin_init(&spin, 0);

	err = pthread_create(&tid, NULL, reader, &spin);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	sched_yield();

	err = pthread_create(&tid, NULL, writer, &spin);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}


	// TODO: join threads

	pthread_exit(NULL);
	pthread_spin_destroy(&spin);

	return 0;
}
