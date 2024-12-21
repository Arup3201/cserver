#ifndef SERVER_H
#define SERVER_H

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
	RequestLine start_line;
	RequestHeader header;
} HttpRequest;

typedef struct {
	StatusLine start_line;
	ResponseHeader header;
} HttpResponse;

#endif
