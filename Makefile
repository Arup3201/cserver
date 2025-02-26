CC=gcc
CFLAGS=-Wall -Wextra
DEPS=http/http.h connection/connection.h

server: server.c http/http.c connection/connection.c
	$(CC) -o server server.c http/http.c connection/connection.c $(CFLAGS)
