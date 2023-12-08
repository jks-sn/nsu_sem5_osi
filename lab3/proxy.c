#include "proxy.h"

void socket_routine(int client_fd) {
	char buf[SOCKET_LINESIZE], uri[SOCKET_LINESIZE], version[SOCKET_LINESIZE], new_request[SOCKET_BUFSIZE];
	sockett_t socket;
	content_t content;
	int host_catched, server_fd;

	sockett_init(&socket, client_fd);
	
	Sockett_readline(&socket, buf, SOCKET_LINESIZE);
    
    //printf("Header: %s\n", buf);
	
	sscanf(buf, "%s %s %s", content.method, uri, content.version);

    parse_uri(uri, &content);
    
    //printf("Path: %s\nHost: %s\nPort: %d\n", content.path, content.host, content.port);
    
    build_server_request(&socket, new_request, &content);
    printf("Request: \n%s", new_request);
    server_fd = Open_server(content.host, content.port);

    Sockett_write(server_fd, new_request, sizeof(new_request));
    
    get_response(server_fd, client_fd);
    close(server_fd);
}   
void get_response(int server_fd, int client_fd) {
    char response[SOCKET_BUFSIZE];
    int response_size;
    do {
        response_size = Fd_read(server_fd, response, SOCKET_BUFSIZE);
        //printf("Response: \n%s", response);
	    Sockett_write(client_fd, response, response_size);
    } while(response_size > 0);
    close(server_fd);
}
void parse_uri(char *uri, content_t *content) {
	char temp[SOCKET_LINESIZE];

	if(strstr(uri,"http://") != NULL) {
	    sscanf(uri, "http://%[^/]%s", temp, content->path); 
    }
	else {
		sscanf(uri, "%[^/]%s", temp, content->path); 
    }

	if(strstr(temp, ":") != NULL)
	    sscanf(temp,"%[^:]:%d", content->host, &content->port);	
	else {
		strcpy(content->host,temp);
		content->port = 80;
	}

	if(!content->path[0])
		strcpy(content->path,"./");
}
void build_server_request(sockett_t *socket, char* new_request, content_t* content) {
    char buf[SOCKET_LINESIZE];
    sprintf(new_request, "%s %s %s\r\n", content->method, content->path, content->version);
    do {
    	Sockett_readline(socket, buf, SOCKET_LINESIZE);
        //printf("Line: %s\n", buf);
        strcat(new_request, buf);
    } while(strcmp(buf, "\r\n"));
}
void sockett_init(sockett_t *socket, int fd) {
    socket->fd = fd;  
    socket->byte_wrote = 0;  
    socket->bufptr = socket->buf;
}
ssize_t sockett_read(sockett_t *socket, char *usrbuf, ssize_t n) {
    int byte_to_return;

    if (socket->byte_wrote <= 0) { 
        socket->byte_wrote = read(socket->fd, socket->buf, sizeof(socket->buf));
        if (socket->byte_wrote < 0) {
                return -1;
            }
        else if (socket->byte_wrote == 0) {  /* EOF */
            return 0;
        }
        else { 
            socket->bufptr = socket->buf;
        }
    }

    byte_to_return = (socket->byte_wrote < n ? socket->byte_wrote : n);          
    memcpy(usrbuf, socket->bufptr, byte_to_return);
    socket->bufptr += byte_to_return;
    socket->byte_wrote -= byte_to_return;
    return byte_to_return;
}
ssize_t sockett_readline(sockett_t *socket, char *output, ssize_t maxlen) {
    ssize_t n, return_value;
    char c;
    for (n = 1; n < maxlen; ++n) { 
        if ((return_value = sockett_read(socket, &c, 1)) == 1) {
            *(output++) = c;
            if (c == '\n') {
                break;
            }
        } 
        else if (return_value == 0) {
            if (n == 1) {
                return 0; /* EOF, no data read */
            }
            else {
                break;    /* EOF, some data was read */
            }
        } else {
            return -1;    /* error */
        }
    }
    *output = 0;
    return n;
}

ssize_t Sockett_readline(sockett_t *socket, void *output, ssize_t maxlen) {
    ssize_t return_value;

    if ((return_value = sockett_readline(socket, output, maxlen)) < 0)
    {
        if(errno==ECONNRESET) {
            pthread_exit(NULL);
        }
        else {
            error1("socket_readline error");
        }
    }
    return return_value;
} 
ssize_t fd_read(int fd, void *usrbuf, ssize_t n) {
    
    ssize_t number_byte_need_read = n;
    ssize_t number_byte_done_read;
    char *bufp = usrbuf;

    while (number_byte_need_read > 0) {
        if ((number_byte_done_read = read(fd, bufp, number_byte_need_read)) < 0) {
            if (errno == EINTR) {
                number_byte_done_read = 0; // get nothing from read()
                break;
            }
            else {
                return -1;
            }
        } 
        else if (number_byte_done_read == 0) {
            break;              /* EOF */
        }
        number_byte_need_read -= number_byte_done_read;
        bufp += number_byte_done_read;
    }
    return (n - number_byte_need_read);
}
ssize_t Fd_read(int fd, void *output, ssize_t n) {
    ssize_t return_value;

    if ((return_value = fd_read(fd, output, n)) < 0) {
        if(errno==ECONNRESET) {
            return 0;;
        }
        else {
            error1("fd_read error");
        }
    }
    return return_value;
}
ssize_t sockett_write(int fd, void *message, ssize_t n) {
    ssize_t byte_need_to_write = n;
    ssize_t byte_wrote = 0;
    char *bufp = message;

    while (byte_need_to_write > 0) {
        if ((byte_wrote = write(fd, bufp, byte_need_to_write)) <= 0) {
            if (errno == EINTR) {  
                byte_wrote = 0;
            }
            else {
                return -1;
            }
        }
        byte_need_to_write -= byte_wrote;
        bufp += byte_wrote;
    }
    return n;
}
void Sockett_write(int fd, void *message, ssize_t n) 
{
    if (sockett_write(fd, message, n) != n)
    {
        if(errno==EPIPE) {
            pthread_exit(NULL);
        }
        else {
            error1("sockett_write error");
        }
    }
        
}
int open_socket(int port) {
    int fd;
    struct sockaddr_in serveraddr;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    serveraddr.sin_family = AF_INET; 
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serveraddr.sin_port = htons((unsigned short)port);

    if (bind(fd, (struct sockaddr*) (&serveraddr), sizeof(struct sockaddr_in)) < 0) {
        return -1;
    }

    if (listen(fd, NUMBER_USERS) < 0) {
        return -1;
    }

    return fd;
}

int Open_socket(int port) {
    int return_value;

    if ((return_value = open_socket(port)) < 0) {
        error1("open_socket error");
    }
    return return_value;
}

int open_server(char *hostname, int port) {
    int server_fd;
    struct addrinfo *addrlist, *addr;
    char port_str[SOCKET_LINESIZE];
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    /* Get a list of addrinfo structs */
    sprintf(port_str, "%d", port);
    if (getaddrinfo(hostname, port_str, NULL, &addrlist) != 0) {
        return -1;
    }
    /* Walk the list, using each addrinfo to try to connect */
    for (addr = addrlist; addr; addr = addr->ai_next) {
        if ((addr->ai_family == AF_INET)) {
            if (connect(server_fd, addr->ai_addr, addr->ai_addrlen) == 0) {
                break; /* success */
            }
        }
    } 
    freeaddrinfo(addrlist);
    if (!addr) { 
        close(server_fd);
        return -1;
    }
    else {
        return server_fd;
    }
}
int Open_server(char *hostname, int port) {
    int return_value;

    if ((return_value = open_server(hostname, port)) < 0) {
        error1("open_server error");
    }
    return return_value;
}
int Accept(int fd, struct sockaddr *addr, socklen_t *addrlen) {
    int return_value;

    if ((return_value = accept(fd, addr, addrlen)) < 0) {
        error1("accept error");
    }
    return return_value;
}
void* Malloc(ssize_t size) {
    void *p;

    if ((p  = malloc(size)) == NULL) {
        error1("malloc error");
    }
    return p;
}
void* thread_routine(void* args) {
	int fd = *((int*) args);
	free(args);
	Pthread_detach(pthread_self());
    //printf("thread_routine - fd: %d\n", fd);
	socket_routine(fd);
	Close(fd);
	return NULL;
}

void Close(int fd) {
    if (close(fd) < 0) {
        error1("close error");
    }
}

void Pthread_create(pthread_t *tid, pthread_attr_t *attr, void * (*routine)(void *), void *args) {
    int return_value;
    //printf("Pthread_create arg: %d\n", *((int*)args));
    if ((return_value = pthread_create(tid, attr, routine, args)) != 0) {
        error2(return_value, "pthread_create error");
    }
}
void Pthread_detach(pthread_t tid) {
    int return_value;

    if ((return_value = pthread_detach(tid))) {
        error2(return_value, "pthread_detach error");
    }
}
void error1(char* str) {
    fprintf(stderr, "%s: %s\n", str, strerror(errno));
    exit(0);
}

void error2(int code, char *str) {
    fprintf(stderr, "%s: %s\n", str, strerror(code));
    exit(0);
}