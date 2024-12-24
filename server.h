#ifndef SERVER_H
#define SERVER_H
#define BUFFER_SIZE 1024

#include<regex.h>
#include<string.h>

typedef struct {
	char *method; // GET POST DELETE PUT PATCH OPTIONS HEAD CONNECT TRACE
	char *request_target;
	char *protocol;
} RequestLine;

typedef struct {
	char *protocol;
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

// utils
void set_http_word(int *word_start, int *word_end, char* str, char* copy_to) {
	int ws = *word_start, we = *word_end;
	ws = we;
	while((str[we] != ' ') && (str[we] != '\r')) we++;

	strncpy(copy_to, str+ws, we-ws);
	copy_to[we-ws] = '\0';

	if(str[we] == ' ') we += 1;
	else if(str[we] == '\r') we += 2;

	*word_start = ws;
	*word_end = we;
}

// function to get http request structure from request string
HttpRequest* get_http_request(char *request_string) {
	// start-line: <method> <request-target> <protocol> e.g. GET / HTTP/1.1
	HttpRequest* request = (HttpRequest*)malloc(sizeof(HttpRequest));
	request->start_line = (RequestLine*)malloc(sizeof(RequestLine));

	/*
	 * ws => word start & we => word end
	 * GET -> ws=0 we=3
	 * we-ws is the length of each string
	 * Next request string will be request_string+ws => GET /home/etc
	*/

	int ws=0, we=0;
	request->start_line->method = (char*)malloc(10);
	request->start_line->request_target= (char*)malloc(128);
	request->start_line->protocol = (char*)malloc(32);
	set_http_word(&ws, &we, request_string, request->start_line->method);
	set_http_word(&ws, &we, request_string, request->start_line->request_target);
	set_http_word(&ws, &we, request_string, request->start_line->protocol);

	// Regex search the header options
	regex_t preg;
	size_t nmatch=1;
	regmatch_t pmatch[nmatch];


	char *exp_str = "Access-Control-Origin";
	regcomp(&preg, exp_str, 0);
	regexec(&preg, request_string, nmatch, pmatch, 0);

	return request;
}


#endif
