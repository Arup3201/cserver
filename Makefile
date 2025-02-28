CC=gcc
CFLAGS=-Wall -Wextra

server: server.c http/http.c connection/connection.c
	$(CC) -o server server.c http/http.c connection/connection.c $(CFLAGS)
