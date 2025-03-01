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

void http_response_error(http_response_t hres) {
	char *error_message = "Error: resource not found\n";

	hres->status_code = HTTP_RESPONSE_CODE_NOT_FOUND;
	strcpy(hres->status_text, HTTP_RESPONSE_TEXT_NOT_FOUND);
	strcpy(hres->content_type, HTTP_HEADER_CONTENT_TYPE_HTML);
	hres->content_length = strlen(error_message);

	strcpy(hres->body, error_message);
}

char* http_get_response_content_type(char *resource) {
	char *resource_name = strrchr(resource, '/') + 1;
	char *extension = strrchr(resource_name, '.') + 1;
	fprintf(stdout, "INFO: Requested content extension - %s\n", extension);
	if(extension == NULL) {
		return HTTP_HEADER_CONTENT_TYPE_JSON;
	}
	
	if(strcmp(extension, "css") == 0) {
		return HTTP_HEADER_CONTENT_TYPE_CSS;
	}

	if(strcmp(extension, "js") == 0) {
		return HTTP_HEADER_CONTENT_TYPE_JAVASCRIPT;
	}

	return HTTP_HEADER_CONTENT_TYPE_HTML;
}

void http_serve_resource(char *filename, http_response_t hres) {
		int fd = open(filename, O_RDONLY, S_IRUSR);
		if(fd < 0) {
			http_response_error(hres);
		}
		
		hres->status_code = HTTP_RESPONSE_CODE_OK;
		strcpy(hres->status_text, HTTP_RESPONSE_TEXT_OK);

		size_t file_length = lseek(fd, 0, SEEK_END);
		hres->content_length = file_length;
		
		lseek(fd, 0, SEEK_SET);
		read(fd, hres->body, file_length);
}

http_response_t http_make_response(http_request_t req) {
	http_response_t hres = (http_response_t)malloc(sizeof(struct http_response));

	strcpy(hres->protocol, HTTP_PROTOCOL_VERSION);

	// fetching the requested resource
	if(strcmp(req->request_target, "/") == 0) {
		char* filename = "index.html";
		http_serve_resource(filename, hres);
	} else {
		char curr_dir[1024];
		if(getcwd(curr_dir, sizeof(curr_dir)) == NULL) {
			fprintf(stderr, "ERROR: failed to get current directory\n");
			exit(EXIT_FAILURE);
		}
		char *filename = strcat(curr_dir, req->request_target);
	
		http_serve_resource(filename, hres);
		strcpy(hres->content_type, http_get_response_content_type(req->request_target));
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
