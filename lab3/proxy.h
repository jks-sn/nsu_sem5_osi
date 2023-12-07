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
#include <arpa/inet.h>

#define PORT 80
#define	SOCKET_LINESIZE 8192  
#define SOCKET_BUFSIZE 8192
#define NUMBER_USERS 1024

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

// static const char *connection_hdr = "Connection: close\r\n";
// static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:120.0) Gecko/20100101 Firefox/120.0\r\n";
// static const char *accept_hdr = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
// static const char *accept_encoding_hdr = "Accept-Encoding: gzip, deflate\r\n";
// static const char *proxy_conn_hdr = "Proxy-Connection: close\r\n";

int Accept(int fd, struct sockaddr *addr, socklen_t *addrlen);
void Close(int fd);

void thread_routine(void *args);
void Pthread_create(pthread_t *tid, pthread_attr_t *attr, void * (*routine)(void *), void *args);
void Pthread_detach(pthread_t tid);

void sockett_init(sockett_t *socket, int fd);
void socket_routine(int server_fd);
ssize_t Sockett_readline(sockett_t *socket, void *output, ssize_t maxlen);
ssize_t sockett_readline(sockett_t *socket, char *output, ssize_t maxlen);
ssize_t fd_read(int fd, void *usrbuf, ssize_t n);
ssize_t Fd_read(int fd, void *output, ssize_t n);
ssize_t sockett_write(int fd, void *output, ssize_t n);
void Sockett_write(int fd, void *output, ssize_t n);
int open_socket(int port);
int Open_socket(int port);
int open_server(char *hostname, int port);
int Open_server(char *hostname, int port);

void build_server_request(char* new_request, content_t* content, char* host_data, int host_catched);
void parse_uri(char *uri, content_t *content);
int get_request(sockett_t *socket, char *data);

void *Malloc(ssize_t size);

void error1(char* str);
void error2(int code, char *str);
void error_client(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);

#endif //PROXY_H