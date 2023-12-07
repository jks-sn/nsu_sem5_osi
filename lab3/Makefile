CC = gcc
CFLAGS = -g -O0
LIBS = -lpthread

all: proxy

proxy: proxy.c main.c
	${CC} ${CFLAGS} proxy.c main.c ${LIBS} -o proxy

clean:
	rm -f proxy

rebuild: clean all