CC=gcc
CFLAGS= -g -O0 -Wall -Wextra
LIBS=-lpthread
QUEUE= queue.h queue.c

SOURCES_2_3_SPINLOCK=$(wildcard 2_3/spinlock/*.c)
SOURCES_2_3_MUTEX=$(wildcard 2_3/mutex/*.c)
SOURCES_2_3_RWLOCK=$(wildcard 2_3/rwlock/*.c)

all: queue-example queue-threads queue-threads1 queue-threads2 queue-threads3 2_3_spinlock 2_3_mutex 2_3_rwlock

queue-example: ${QUEUE} queue-example.c
	${CC} ${CFLAGS} ${QUEUE} queue-example.c ${LIBS} -o queue-example

queue-threads: ${QUEUE} queue-threads.c
	${CC} ${CFLAGS} ${QUEUE} queue-threads.c ${LIBS} -o queue-threads

queue-threads1: ${QUEUE} queue-threads1.c
	${CC} ${CFLAGS} ${QUEUE} queue-threads1.c ${LIBS} -o queue-threads1

queue-threads2: ${QUEUE} queue-threads2.c
	${CC} ${CFLAGS} ${QUEUE} queue-threads2.c ${LIBS} -o queue-threads2

queue-threads3: ${QUEUE} queue-threads3.c
	${CC} ${CFLAGS} ${QUEUE} queue-threads3.c ${LIBS} -o queue-threads3
2_3_spinlock:  ${QUEUE} ${SOURCES_2_3_SPINLOCK}
	${CC} ${CFLAGS} ${QUEUE} ${SOURCES_2_3_SPINLOCK} ${LIBS} -o 2_3_spinlock
2_3_mutex:  ${QUEUE} ${SOURCES_2_3_MUTEX}
	${CC} ${CFLAGS} ${QUEUE} ${SOURCES_2_3_MUTEX} ${LIBS} -o 2_3_mutex
2_3_rwlock:  ${QUEUE} ${SOURCES_2_3_RWLOCK}
	${CC} ${CFLAGS} ${QUEUE} ${SOURCES_2_3_RWLOCK} ${LIBS} -o 2_3_rwlock
clean:
	rm -f *.o queue-example queue-threads queue-threads1 queue-threads2 queue-threads3 2_3_spinlock 2_3_mutex 2_3_rwlock

rebuild: clean all
