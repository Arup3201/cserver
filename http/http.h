#ifndef HTTP_H
#define HTTP_H

typedef struct {
	char *method; // GET POST DELETE PUT PATCH OPTIONS HEAD CONNECT TRACE
	char *request_target;
	char *protocol;
} http_request_line_t;

typedef struct {
	char *protocol;
	int status_code;
	char *status_text;
} http_status_line_t;

typedef struct {
	char *host;
	char *user_agent;
	char *accept;
	char *content_type;
	int content_length;
	char *connection;
} http_request_header_t;

typedef struct {
	char *access_control_origin;
	char *connection;
	char *content_encoding;
	char *content_type;
	char *keep_alive;
	char *server;
	char *set_cookie;
} http_response_header_t;

typedef struct {
	http_request_line_t *start_line;
	http_request_header_t *header;
} http_request_t;

typedef struct {
	http_status_line_t *start_line;
	http_response_header_t *header;
} http_response_t;

http_request_t* get_http_request(char*);
void print_http_request(http_request_t*);
void free_request(http_request_t*);

#endif
