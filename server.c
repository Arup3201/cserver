#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>

int main() {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	printf("fd=%d\n", fd);
	return 0;
}
