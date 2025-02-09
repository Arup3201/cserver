#ifndef HTTP_H
#define HTTP_H

// HTTP methods
#define HTTP_METHOD_GET "GET"
#define HTTP_METHOD_HEAD "HEAD"
#define HTTP_METHOD_OPTIONS "OPTIONS"
#define HTTP_METHOD_TRACE "TRACE"
#define HTTP_METHOD_PUT "PUT"
#define HTTP_METHOD_DELETE "DELETE"
#define HTTP_METHOD_POST "POST"
#define HTTP_METHOD_PATCH "PATCH"
#define HTTP_METHOD_CONNECT "CONNECT"

// HTTP protocol
#define HTTP_PROTOCOL_VERSION "HTTP/1.1"

// HTTP status codes
#define HTTP_RESPONSE_CODE_OK 200
#define HTTP_RESPONSE_CODE_CREATED 201
#define HTTP_RESPONSE_CODE_BAD_REQUEST 400
#define HTTP_RESPONSE_CODE_UNAUTHORIZED 401
#define HTTP_RESPONSE_CODE_FORBIDDEN 403
#define HTTP_RESPONSE_CODE_NOT_FOUND 404
#define HTTP_RESPONSE_CODE_METHOD_NOT_ALLOWED 405
#define HTTP_RESPONSE_CODE_REQUEST_TIMEOUT 408
#define HTTP_RESPONSE_CODE_INTERNAL_SERVER_ERROR 500
#define HTTP_RESPONSE_CODE_BAD_GATEWAY 502
#define HTTP_RESPONSE_CODE_SERVICE_UNAVAILABLE 503

// HTTP status texts
#define HTTP_RESPONSE_TEXT_OK "OK"
#define HTTP_RESPONSE_TEXT_CREATED "Created"
#define HTTP_RESPONSE_TEXT_BAD_REQUEST "Bad Request"
#define HTTP_RESPONSE_TEXT_UNAUTHORIZED "Unauthorized"
#define HTTP_RESPONSE_TEXT_FORBIDDEN "Forbidden"
#define HTTP_RESPONSE_TEXT_NOT_FOUND "Not Found"
#define HTTP_RESPONSE_TEXT_METHOD_NOT_ALLOWED "Method Not Allowed"
#define HTTP_RESPONSE_TEXT_REQUEST_TIMEOUT "Request Timeout"
#define HTTP_RESPONSE_TEXT_INTERNAL_SERVER_ERROR "Internal Server Error"
#define HTTP_RESPONSE_TEXT_BAD_GATEWAY "Bad Gateway"
#define HTTP_RESPONSE_TEXT_SERVICE_UNAVAILABLE "Service Unavailable"

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
	char* body;
} http_response_t;


// http functions - http_fn_[function name]
http_request_t* http_fn_get_request(char*); // set request data from client
char* http_fn_get_response(http_request_t*); // send response to client based on the requested content
void http_fn_print_request(http_request_t*); // print request
void http_fn_print_response(http_response_t*); // show response
void http_fn_free_request(http_request_t*); // free the allocated space of request
void http_fn_free_response(http_response_t*); // free the allocated space of response 

#endif
