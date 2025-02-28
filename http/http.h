#ifndef HTTP_H
#define HTTP_H


/**
 * Messages in HTTP/1.1 are text based and here we are going to use them
 * Messages in HTTP has 4 parts - start line, headers, empty line and body
 *
 * start line for request has - <method> <request-target> <protocol>
 * start line for response has - <protocol> <status-code> <status-text>
 *
 * <request-target>: 
 * - origin form - GET /EN-US/docs/webs/Messages HTTP/1.1
 * - absolute form - GET https://developer.mozilla.org/EN-US/docs/webs/Messages HTTP/1.1
 * - authority form - CONNECT developer.mozilla.org:403 HTTP/1.1
 * - asterisk form - OPTIONS * HTTP/1.1
 */

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

// HTTP request target types
enum HTTP_REQUEST_TARGET_TYPE {
	ORIGIN_FORM, 
	ABSOLUTE_FORM, 
	AUTHORITY_FORM, 
	ASTERISK_FORM
};

// HTTP Headers
#define HTTP_HEADER_HOST "Host"
#define HTTP_HEADER_CONTENT_TYPE "Content-Type"
#define HTTP_HEADER_CONTENT_LENGTH "Content-Length"
#define HTTP_HEADER_USER_AGENT "User-Agent"

// HTTP Content Types
#define HTTP_HEADER_CONTENT_TYPE_JSON "application/json"
#define HTTP_HEADER_CONTENT_TYPE_HTML "text/html"
#define HTTP_HEADER_CONTENT_TYPE_CSS "text/css"
#define HTTP_HEADER_CONTENT_TYPE_JAVASCRIPT "text/javascript"
#define HTTP_HEADER_CONTENT_TYPE_FORM "application/x-www-form-urlencoded"

// generate the header
#define GET_HEADER(header, value) #header ": "  #value "\r\n"

typedef struct http_request {
	// start line
	char method[16];
	char request_target[128];
	char protocol[16];

	// header
	char host[128];
	char content_type[128];
	char user_agent[128];
	int content_length;
	
	// body
	char body[1024];
} *http_request_t;

typedef struct http_response {
	char protocol[16];
	int status_code;
	char status_text[128];

	char host[128];
	char user_agent[128];
	char content_type[128];
	int content_length;

	char body[1024];
} *http_response_t;

http_request_t http_make_request(char[], int); // generate HTTP request
http_response_t http_make_response(http_request_t); // genereate HTTP response
void http_get_response_string(http_response_t, char*, int); // convert the response to string
void http_free_request(http_request_t); // free request memory
void http_free_response(http_response_t); // free response memory

#endif
