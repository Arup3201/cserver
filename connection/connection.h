#ifndef CONNECTION_H
#define CONNECTION_H

#ifndef	_SYS_SOCKET_H
#include<sys/socket.h>
#endif

#ifndef	_NETINET_IN_H
#include<netinet/in.h>
#endif

typedef struct host{
	int fd;
	struct sockaddr_in addr;
	socklen_t length;
}*host_t;

typedef void (*response_handler_t)(int*) ;

host_t conn_get_server(char*, int);
void connn_response_loop(host_t, response_handler_t);

#endif // !CONNECTION_H
