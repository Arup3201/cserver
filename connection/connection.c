#include "connection.h"

#ifndef _STDIO_H
#include<stdio.h>
#endif

#ifndef	_STDLIB_H
#include<stdlib.h>
#endif

#ifndef _ARPA_INET_H
#include<arpa/inet.h>
#endif

host_t conn_get_server(char* address, int port) {
	host_t server = (host_t)malloc(sizeof(struct host));

	server->fd = socket(PF_INET, SOCK_STREAM, 0);

	server->addr.sin_family = AF_INET;
	server->addr.sin_port = htons(port);

	if(inet_aton(address, &server->addr.sin_addr) == 0) {
		perror("inet-aton");
		exit(EXIT_FAILURE);
	}

	if(bind(server->fd, (struct sockaddr*)&server->addr, server->length) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	return server;
}

void conn_response_loop(host_t server, response_handler_t handler) {
	if(listen(server->fd, 1) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	host_t client = (host_t)malloc(sizeof(struct host));

	while(1) {
		client->fd = accept(server->fd, (struct sockaddr*)&client->addr, &client->length);
		if(client->fd < 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}

		printf("Server accepted a client\n");
		(*handler)(&client->fd);
	}
}
