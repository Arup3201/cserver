#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main() {
	// Create the socket for client
	int sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	if(sock_fd < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in name;
	name.sin_family = AF_INET;
	name.sin_port = htons(6666);
	if(inet_aton("127.0.0.1", &name.sin_addr) == 0) {
		perror("inet");
		exit(EXIT_FAILURE);
	}

	if(connect(sock_fd, (struct sockaddr*)&name, sizeof(name)) < 0) {
		perror("connect");
		exit(EXIT_FAILURE);
	}
	printf("Client connected with server\n");

	size_t len = 1024;
	char buffer[len];
	memset(buffer, '\0', len);
	ssize_t nbytes = recv(sock_fd, buffer, len, 0);
	printf("Server: %s\n", buffer);
	printf("%ld bytes received from server.\n", nbytes);

	return 0;
}
