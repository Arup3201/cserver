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

#ifndef _STRING_H
#include<string.h>
#endif

#ifndef _UNISTD_H
#include<unistd.h>
#endif

conn_host_t conn_get_host(char* address, int port) {
	conn_host_t host = (conn_host_t)malloc(sizeof(struct conn_host));

	host->fd = socket(AF_INET, SOCK_STREAM, 0);

	host->addr.sin_family = AF_INET;
	host->addr.sin_port = htons(port);

	if(inet_aton(address, &host->addr.sin_addr) == 0) {
		perror("inet-aton");
		exit(EXIT_FAILURE);
	}

	// this host can be attached to an address which is already in use
	// FOR TESTING
	if(setsockopt(host->fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) > 0) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	host->length = sizeof(host->addr);
	if(bind(host->fd, (struct sockaddr*)&host->addr, host->length) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	return host;
}

void conn_handle_client_request(conn_host_t host, request_handler_t handler) {
	if(listen(host->fd, 2) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}


	conn_host_t client = (conn_host_t)malloc(sizeof(struct conn_host));
	conn_message_t request = (conn_message_t)malloc(sizeof(struct conn_message));
	request->length = CONNECTION_MESSAGE_MAX_LEN;
	conn_message_t response = (conn_message_t)malloc(sizeof(struct conn_message));

	while(1) {
		client->fd = accept(host->fd, (struct sockaddr*)&client->addr, &client->length);
		if(client->fd < 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}

		printf("Server accepted a client\n");


		ssize_t nbytes = recv(client->fd, request->message, request->length, 0);
		if(nbytes > 0) {
			response = (*handler)(request);
			send(client->fd, response->message, response->length, 0);
		} else  {
			fprintf(stdout, "Client disconnected!\n");
		}

		shutdown(client->fd, SHUT_RDWR);

		memset(request->message, '\0', CONNECTION_MESSAGE_MAX_LEN);
		memset(response->message, '\0', CONNECTION_MESSAGE_MAX_LEN);
		response->length = 0;
	}
	close(host->fd);
	close(client->fd);

	free(client);
	free(request);
	free(response);
}
