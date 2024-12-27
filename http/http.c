#include<stdio.h>
#include<stdlib.h>
#include<regex.h>
#include<string.h>
#include "http.h"

#define BUFFER_SIZE 1024

// get start line of http request and response
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

// get header value for a given header name
char* get_hv(char* h_name, char* str, size_t *len) {
	regex_t reg;
	size_t nmatch=2;
	regmatch_t pmatch[nmatch];

	if(regcomp(&reg, h_name, 0) != 0) {
		return NULL;
	}
	if(regexec(&reg, str, nmatch, pmatch, 0) == REG_NOMATCH) {
		return NULL;
	}

	// header value starts at - +2 for :<space>
	size_t hv_start, hv_end;

	hv_start = pmatch[0].rm_so + strlen(h_name) + 2;

	// header value ends at - it is exclusive
	hv_end=hv_start;
	while(str[hv_end] != '\r') hv_end++;

	*len = hv_end - hv_start;

	// free reg object
	regfree(&reg);

	return str+hv_start;
}

void print_http_request(HttpRequest *req) {
	printf("=====HTTP Request=====\n");
	printf("%s %s %s\r\n", req->start_line->method, req->start_line->request_target, req->start_line->protocol);
	printf("Host: %s\r\n", req->header->host);
	printf("User-Agent: %s\r\n", req->header->user_agent);
	printf("Connection: %s\r\n", req->header->connection);
	printf("Accept: %s\r\n",  req->header->accept);
	printf("Content-Type: %s\r\n", req->header->content_type);
	printf("Content-Length: %d\r\n", req->header->content_length);
	printf("======================\n\n");
}

// function to get http request structure from request string
HttpRequest* get_http_request(char *request_string) {
	// start-line: <method> <request-target> <protocol> e.g. GET / HTTP/1.1
	HttpRequest* request = (HttpRequest*)malloc(sizeof(HttpRequest));
	request->start_line = (RequestLine*)malloc(sizeof(RequestLine));
	request->header = (RequestHeader*)malloc(sizeof(RequestHeader));

	int ws=0, we=0;
	request->start_line->method = (char*)malloc(10);
	request->start_line->request_target= (char*)malloc(128);
	request->start_line->protocol = (char*)malloc(32);
	set_http_word(&ws, &we, request_string, request->start_line->method);
	set_http_word(&ws, &we, request_string, request->start_line->request_target);
	set_http_word(&ws, &we, request_string, request->start_line->protocol);

	request->header->host = (char*)malloc(128);
	request->header->user_agent = (char*)malloc(128);
	request->header->connection = (char*)malloc(128);
	request->header->accept = (char*)malloc(128);
	request->header->content_type = (char*)malloc(128);

	size_t hv_len;
	char* str;

	str = get_hv("Host", request_string, &hv_len);
	if(str != NULL) {
		strncpy(request->header->host, str, hv_len);
		request->header->host[hv_len] = '\0';	
	}
	str = get_hv("User-Agent", request_string, &hv_len);
	if(str != NULL) {
		strncpy(request->header->user_agent, str, hv_len);
		request->header->user_agent[hv_len] = '\0';
	}
	str = get_hv("Accept", request_string, &hv_len);
	if(str != NULL) {
		strncpy(request->header->accept, str, hv_len);
		request->header->accept[hv_len] = '\0';
	}

	str = get_hv("Content-Type", request_string, &hv_len);
	if(str != NULL) {
		strncpy(request->header->content_type, str, hv_len);
		request->header->content_type[hv_len] = '\0';
	}

	char* content_length = (char*)malloc(128);
	str = get_hv("Content-Length", request_string, &hv_len);
	if(str != NULL) {
		strncpy(content_length, str, hv_len);
		content_length[hv_len] = '\0';
		request->header->content_length = atoi(content_length);
	}
	
	str = get_hv("Connection", request_string, &hv_len);
	if(str != NULL) {
		strncpy(request->header->connection, str, hv_len);
		request->header->connection[hv_len] = '\0';
	}
	return request;
}

void free_request(HttpRequest* req) {
	free(req->start_line->method);
	free(req->start_line->request_target);
	free(req->start_line->protocol);
	free(req->start_line);

	free(req->header->host);
	free(req->header->user_agent);
	free(req->header->connection);
	free(req->header->accept);
	free(req->header->content_type);
	free(req->header);

}
