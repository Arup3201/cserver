#include "http.h"
#include <unistd.h>

#ifndef	_STDLIB_H
#include<stdlib.h>
#endif

#ifndef _STRING_H
#include <string.h>
#endif

#ifndef _STDIO_H
#include<stdio.h>
#endif

#ifndef	_FCNTL_H
#include<fcntl.h>
#endif

http_request_t http_make_request(char *req, int req_length) {
	http_request_t hreq = (http_request_t)malloc(sizeof(struct http_request));

	// Extract the method, request target and protocol from - GET / HTTP/1.1 or POST /signup HTTP/1.1 etc
	char* first_space = strchr(req, ' ');
	int method_str_length = first_space - req;
	strncpy(hreq->method, req, method_str_length);
	hreq->method[method_str_length] = 0;

	char* second_space = strchr(first_space + 1, ' ');
	int path_str_length = second_space - first_space - 1;
	strncpy(hreq->request_target, first_space+1, path_str_length);
	hreq->request_target[path_str_length] = 0;

	char* line_end = strchr(second_space + 1, '\r');
	int protocol_str_length = line_end - second_space - 1;
	strncpy(hreq->protocol, second_space+1, protocol_str_length);


	return hreq;
}

http_response_t http_response_error() {
	http_response_t hres = (http_response_t)malloc(sizeof(struct http_response));

	hres->status_code = HTTP_RESPONSE_CODE_NOT_FOUND;
	strcpy(hres->status_text, HTTP_RESPONSE_TEXT_NOT_FOUND);
	strcpy(hres->content_type, HTTP_HEADER_CONTENT_TYPE_HTML);
	strcpy(hres->body, "Error: resource not found\n");
	hres->content_length = strlen(hres->body);

	return hres;
}

http_response_t http_make_response(http_request_t req) {
	http_response_t hres = (http_response_t)malloc(sizeof(struct http_response));

	strcpy(hres->protocol, HTTP_PROTOCOL_VERSION);

	// fetching the requested resource
	if(strcmp(req->request_target, "/") == 0) {
		char* filename = "index.html";
		int fd = open(filename, O_RDONLY, S_IRUSR);
		if(fd < 0) {
			fprintf(stderr, "Errro: can't server file index.html\n");
			return http_response_error();
		}
		
		hres->status_code = HTTP_RESPONSE_CODE_OK;
		strcpy(hres->status_text, HTTP_RESPONSE_TEXT_OK);
		strcpy(hres->content_type, HTTP_HEADER_CONTENT_TYPE_HTML);
	
		size_t file_length = lseek(fd, 0, SEEK_END);
		fprintf(stdout, "INFO: Total file length of index.html - %ld\n", file_length);
		read(fd, hres->body, file_length);
		hres->content_length = file_length;
	} else {
		return http_response_error();
	}

	return hres;
}

char* http_build_header(char* name, char* value) {
	char* buf = malloc(strlen(name) + strlen(value)+4);
	sprintf(buf, "%s: %s\r\n", name, value);
	return buf;
}

void http_get_response_string(http_response_t res, char* buf, int length) {
	sprintf(buf, "%s %d %s\r\n", res->protocol, res->status_code, res->status_text);
	sprintf(buf, "%s%s", buf, http_build_header(HTTP_HEADER_CONTENT_TYPE, res->content_type));
	sprintf(buf, "%s%s: %d\r\n", buf, HTTP_HEADER_CONTENT_LENGTH, res->content_length);
	sprintf(buf, "%s%s", buf, http_build_header(HTTP_HEADER_CONNECTION, "close"));
	
	sprintf(buf, "%s\r\n", buf);
	sprintf(buf, "%s%s", buf, res->body);
}
