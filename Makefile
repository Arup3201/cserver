CC=gcc
CFLAGS=-Wall

server: server.c http/http.c connection/connection.c
	$(CC) -o server server.c http/http.c connection/connection.c $(CFLAGS)
