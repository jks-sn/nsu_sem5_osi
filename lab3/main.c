#include "proxy.h"

int main() {

	int listen_fd;
	socklen_t clientlen = sizeof(struct sockaddr_in);
	int* socket_fd;
	pthread_t tid;
	struct sockaddr_in clientaddr;
	listen_fd = Open_socket(PORT);

	while(1) {
		socket_fd = (int*)Malloc(sizeof(int));
		*socket_fd = Accept(listen_fd, (struct sockaddr*) (&clientaddr), &clientlen);
		printf("Address: %s; Port: %d; fd: %d\n", inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port, *socket_fd);
		Pthread_create(&tid, NULL, (void *)thread_routine, (void*) socket_fd);	
	}
}