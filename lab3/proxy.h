#ifndef PROXY_H
#define PROXY_H

#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <bits/socket.h>
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>

#define PORT 80
#define	SOCKET_LINESIZE 1<<13  
#define SOCKET_BUFSIZE 1<<16
#define NUMBER_USERS 1<<10

typedef struct {
    int fd;
    ssize_t byte_wrote;
    char *bufptr;
    char buf[SOCKET_BUFSIZE];
}sockett_t;

typedef struct {
    char method[SOCKET_LINESIZE];
	char path[SOCKET_LINESIZE];
    char version[SOCKET_LINESIZE];
    char host[SOCKET_LINESIZE];
	int port;
}content_t;

int Accept(int fd, struct sockaddr *addr, socklen_t *addrlen);
void Close(int fd);

void* thread_routine(void *args);
void Pthread_create(pthread_t *tid, pthread_attr_t *attr, void * (*routine)(void *), void *args);
void Pthread_detach(pthread_t tid);

void get_response(int server_fd, int client_fd);
void sockett_init(sockett_t *socket, int fd);
void socket_routine(int server_fd);
ssize_t sockett_read(sockett_t *socket, char *usrbuf, ssize_t n);
ssize_t Sockett_readline(sockett_t *socket, void *output, ssize_t maxlen);
ssize_t sockett_readline(sockett_t *socket, char *output, ssize_t maxlen);
ssize_t fd_read(int fd, void *usrbuf, ssize_t n);
ssize_t Fd_read(int fd, void *output, ssize_t n);
ssize_t sockett_write(int fd, void *message, ssize_t n);
void Sockett_write(int fd, void *message, ssize_t n);
int open_socket(int port);
int Open_socket(int port);
int open_server(char *hostname, int port);
int Open_server(char *hostname, int port);

void build_server_request(sockett_t *socket, char* new_request, content_t* content);
void parse_uri(char *uri, content_t *content);

void *Malloc(ssize_t size);

void error1(char* str);
void error2(int code, char *str);

#endif //PROXY_H