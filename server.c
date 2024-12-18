#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(){
	// Create a socket
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if(sockfd<0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	// Bind network address and port with the socket
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6666);
	if(inet_aton("127.0.0.1", &addr.sin_addr)==0) {
		perror("inet_aton");
		exit(EXIT_FAILURE);
	}
	socklen_t length = sizeof(addr);
	if(bind(sockfd, (struct sockaddr*)&addr, length) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	// Start listening for clients
	if(listen(sockfd, 1) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	// Accept the client connection
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	int conn_fd = accept(sockfd, (struct sockaddr*)&client, &len);
	if(conn_fd < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	
	// Send a message from server (BUG)
	char *message = "Hello from server";
	send(conn_fd, message, sizeof(message), 0);

	return 0; 
}
