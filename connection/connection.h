#ifndef CONNECTION_H
#define CONNECTION_H

#ifndef	_SYS_SOCKET_H
#include<sys/socket.h>
#endif

#ifndef	_NETINET_IN_H
#include<netinet/in.h>
#endif

typedef struct conn_host{
	int fd;
	struct sockaddr_in addr;
	socklen_t length;
} *conn_host_t;

#define CONNECTION_MESSAGE_MAX_LEN 1024
typedef struct conn_message{
	char message[CONNECTION_MESSAGE_MAX_LEN];
	int length;
} *conn_message_t;

typedef conn_message_t (*request_handler_t)(conn_message_t) ;

conn_host_t conn_get_host(char*, int);
void conn_handle_client_request(conn_host_t, request_handler_t);

#endif // !CONNECTION_H
