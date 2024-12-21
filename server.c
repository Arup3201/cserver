#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<threads.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int send_http_response(void*);

int main(int argc, char* argv[]){
	if(argc < 3) {
		printf("Format: ./server 127.0.0.1 6666\n");
		exit(EXIT_FAILURE);
	}

	char *address = argv[1];
	int port = atoi(argv[2]);

	// Create a socket
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if(sockfd<0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	// Bind network address and port with the socket
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if(inet_aton(address, &addr.sin_addr)==0) {
		perror("inet_aton");
		exit(EXIT_FAILURE);
	}
	socklen_t length = sizeof(addr);
	if(bind(sockfd, (struct sockaddr*)&addr, length) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	// Start listening for clients
	printf("Server is listening at %s:%d\n", address, port);
	if(listen(sockfd, 1) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	

	// Accept the client connection
	struct sockaddr_in client;
	socklen_t len = sizeof(client);

	while(1) {
		int conn_fd = accept(sockfd, (struct sockaddr*)&client, &len);
		if(conn_fd < 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}
		printf("Server accepted a client\n");	
	

		thrd_t p_thread;
		thrd_create(&p_thread, send_http_response, &conn_fd);
		thrd_detach(p_thread);
	}
	return 0; 
}

int send_http_response(void* fd) {
	int *conn_fd = (int*)fd;
	size_t buffer_len = 2048;
	char http_request[buffer_len];
	char http_response[buffer_len];

	// Reset the address with null
	memset(http_response, '\0', buffer_len);
	memset(http_request, '\0', buffer_len);

	ssize_t nbytes = recv(*conn_fd, http_request, buffer_len, 0);
	// Send response to browser
	if(nbytes > 0) {
		strcpy(http_response, "HTTP/1.1 200 OK\r\nConnection: close\r\n");
		strcat(http_response, "\nHello");
		printf("%s\n\n", http_response);
		send(*conn_fd, http_response, buffer_len, 0);
	} else {
		printf("Client disconnected!\n");
	}

	return 1;
}
