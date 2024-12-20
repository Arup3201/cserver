#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

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
	int port = 6666;
	char address[] = "127.0.0.1";
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
	int conn_fd = accept(sockfd, (struct sockaddr*)&client, &len);
	if(conn_fd < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	printf("Server accepted a client\n");

	size_t buffer_len = 1024;
	char buffer[buffer_len];
	memset(buffer, '\0', len);
	scanf("%[^\n]s", buffer);
	send(conn_fd, buffer, len, 0);

	return 0; 
}
