CC=gcc
CFLAGS= -g -O0 -Wall
LIBS=-lpthread
SOURCES_THREAD_1_6=$(wildcard thread1_6/*.c)
all: thread1_1_a thread1_1_b thread1_1_c_1 thread1_1_c_2 thread1_1_d thread1_1_e thread1_2_a thread1_2_b thread1_2_c thread1_2_d thread1_2_e thread1_2_f thread1_3_a thread1_3_b thread1_4_a thread1_4_b thread1_4_c thread1_5_a thread1_6

thread1_1_a: thread1_1_a.c
	${CC} ${CFLAGS} thread1_1_a.c ${LIBS} -o thread1_1_a
thread1_1_b: thread1_1_b.c
	${CC} ${CFLAGS} thread1_1_b.c ${LIBS} -o thread1_1_b
thread1_1_c_1: thread1_1_c_1.c
	${CC} ${CFLAGS} thread1_1_c_1.c ${LIBS} -o thread1_1_c_1
thread1_1_c_2: thread1_1_c_2.c
	${CC} ${CFLAGS} thread1_1_c_2.c ${LIBS} -o thread1_1_c_2
thread1_1_d: thread1_1_d.c
	${CC} ${CFLAGS} thread1_1_d.c ${LIBS} -o thread1_1_d
thread1_1_e: thread1_1_e.c
	${CC} ${CFLAGS} thread1_1_e.c ${LIBS} -o thread1_1_e
thread1_2_a: thread1_2_a.c
	${CC} ${CFLAGS} thread1_2_a.c ${LIBS} -o thread1_2_a
thread1_2_b: thread1_2_b.c
	${CC} ${CFLAGS} thread1_2_b.c ${LIBS} -o thread1_2_b
thread1_2_c: thread1_2_c.c
	${CC} ${CFLAGS} thread1_2_c.c ${LIBS} -o thread1_2_c
thread1_2_d: thread1_2_d.c
	${CC} ${CFLAGS} thread1_2_d.c ${LIBS} -o thread1_2_d
thread1_2_e: thread1_2_e.c
	${CC} ${CFLAGS} thread1_2_e.c ${LIBS} -o thread1_2_e
thread1_2_f: thread1_2_f.c
	${CC} ${CFLAGS} thread1_2_f.c ${LIBS} -o thread1_2_f
thread1_3_a: thread1_3_a.c
	${CC} ${CFLAGS} thread1_3_a.c ${LIBS} -o thread1_3_a
thread1_3_b: thread1_3_b.c
	${CC} ${CFLAGS} thread1_3_b.c ${LIBS} -o thread1_3_b
thread1_4_a: thread1_4_a.c
	${CC} ${CFLAGS} thread1_4_a.c ${LIBS} -o thread1_4_a
thread1_4_b: thread1_4_b.c
	${CC} ${CFLAGS} thread1_4_b.c ${LIBS} -o thread1_4_b
thread1_4_c: thread1_4_c.c
	${CC} ${CFLAGS} thread1_4_c.c ${LIBS} -o thread1_4_c
thread1_5_a: thread1_5_a.c
	${CC} ${CFLAGS} thread1_5_a.c ${LIBS} -o thread1_5_a
thread1_6: $(SOURCES_THREAD_1_6)
	${CC} ${CFLAGS} $(SOURCES_THREAD_1_6) ${LIBS} -o thread1_6.out
clean:
	rm -f *.o thread1_1_a thread1_1_b thread1_1_c_1 thread1_1_c_2 thread1_1_d thread1_1_e thread1_2_a thread1_2_b thread1_2_c thread1_2_d thread1_2_e thread1_2_f thread1_3_a thread1_3_b thread1_4_a thread1_4_b thread1_4_c thread1_5_a thread1_6.out
rebuild: clean all
