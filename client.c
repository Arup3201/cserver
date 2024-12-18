#include<stdio.h>
#include<stdlib.h>

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

	// Recv from server (BUG)
	char *message = "";
	ssize_t nbytes = recv(sock_fd, message, sizeof(message), 0);
	printf("Server: %s\n", message);
	printf("%ld bytes received from server.", nbytes);

	return 0;
}
