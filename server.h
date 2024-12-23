#ifndef SERVER_H
#define SERVER_H

#include<regex.h>
#include<string.h>

typedef struct {
	char method[10]; // GET POST DELETE PUT PATCH OPTIONS HEAD CONNECT TRACE
	char *request_target;
	char protocol;
} RequestLine;

typedef struct {
	char protocol;
	int status_code;
	char *status_text;
} StatusLine;

typedef struct {
	char *host;
	char *user_agent;
	char *accept;
	char *accept_language;
	char *referer;
	char *connection;
	int upgrade_insecure_requests;
} RequestHeader;

typedef struct {
	char *access_control_origin;
	char *connection;
	char *content_encoding;
	char *content_type;
	char *keep_alive;
	char *server;
	char *set_cookie;
} ResponseHeader;

typedef struct {
	RequestLine *start_line;
	RequestHeader *header;
} HttpRequest;

typedef struct {
	StatusLine *start_line;
	ResponseHeader *header;
} HttpResponse;

// function to get http request structure from request string
HttpRequest* get_http_request(char *request_string) {
	// start-line: <method> <request-target> <protocol>
	HttpRequest* request = (HttpRequest*)malloc(sizeof(HttpRequest));
	
	int ws=0, we=0; // word start & word end(exclusive)
	while(request_string[we]!=' ') we++; // The first word length = we-ws, in the request
	request->start_line = (RequestLine*)malloc(sizeof(RequestLine));
	strncpy(request->start_line->method, request_string, we-ws);
	request->start_line->method[we-ws] = '\0';

	return request;	
}


#endif
