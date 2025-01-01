#include<stdio.h>
#include<stdlib.h>
#include<regex.h>
#include<string.h>
#include "http.h"

#define BUFFER_SIZE 1024

// set start line values of request and response
void set_start_line(int *word_start, int *word_end, char* str, char* copy_to) {
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
char* get_header(char* h_name, char* str, size_t *len) {
	regex_t reg;
	size_t nmatch=2;
	regmatch_t pmatch[nmatch];

	if(regcomp(&reg, h_name, 0) != 0) {
		return NULL;
	}
	if(regexec(&reg, str, nmatch, pmatch, 0) == REG_NOMATCH) {
		return NULL;
	}

	size_t hv_start, hv_end;

	// header value starts at - +2 for :<space>
	hv_start = pmatch[0].rm_so + strlen(h_name) + 2;

	// header value ends at (it is exclusive)
	hv_end=hv_start;
	while(str[hv_end] != '\r') hv_end++;

	*len = hv_end - hv_start;

	// free reg object
	regfree(&reg);

	return str+hv_start;
}

void http_fn_print_request(http_request_t *req) {
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

http_request_t* http_fn_get_request(char *request_string) {
	// start-line: <method> <request-target> <protocol> e.g. GET / HTTP/1.1
	http_request_t* request = (http_request_t*)malloc(sizeof(http_request_t));
	request->start_line = (http_request_line_t*)malloc(sizeof(http_request_line_t));
	request->header = (http_request_header_t*)malloc(sizeof(http_request_header_t));

	int ws=0, we=0;
	request->start_line->method = (char*)malloc(10);
	request->start_line->request_target= (char*)malloc(128);
	request->start_line->protocol = (char*)malloc(32);
	set_start_line(&ws, &we, request_string, request->start_line->method);
	set_start_line(&ws, &we, request_string, request->start_line->request_target);
	set_start_line(&ws, &we, request_string, request->start_line->protocol);

	request->header->host = (char*)malloc(128);
	request->header->user_agent = (char*)malloc(128);
	request->header->connection = (char*)malloc(128);
	request->header->accept = (char*)malloc(128);
	request->header->content_type = (char*)malloc(128);

	size_t hv_len;
	char* str;

	str = get_header("Host", request_string, &hv_len);
	if(str != NULL) {
		strncpy(request->header->host, str, hv_len);
		request->header->host[hv_len] = '\0';	
	}
	str = get_header("User-Agent", request_string, &hv_len);
	if(str != NULL) {
		strncpy(request->header->user_agent, str, hv_len);
		request->header->user_agent[hv_len] = '\0';
	}
	str = get_header("Accept", request_string, &hv_len);
	if(str != NULL) {
		strncpy(request->header->accept, str, hv_len);
		request->header->accept[hv_len] = '\0';
	}

	str = get_header("Content-Type", request_string, &hv_len);
	if(str != NULL) {
		strncpy(request->header->content_type, str, hv_len);
		request->header->content_type[hv_len] = '\0';
	}

	char* content_length = (char*)malloc(128);
	str = get_header("Content-Length", request_string, &hv_len);
	if(str != NULL) {
		strncpy(content_length, str, hv_len);
		content_length[hv_len] = '\0';
		request->header->content_length = atoi(content_length);
	}
	
	str = get_header("Connection", request_string, &hv_len);
	if(str != NULL) {
		strncpy(request->header->connection, str, hv_len);
		request->header->connection[hv_len] = '\0';
	}
	return request;
}

void set_response(http_response_t* res, http_request_t* req, char* fcontent) {
	
}

void set_error_response(http_response_t* res, http_request_t* req) {
	strcpy(res->start_line->protocol, req->start_line->protocol);
	res->start_line->status_code = 404;
	strcpy(res->start_line->status_text, "Not Found");
	strcpy(res->header->access_control_origin, "*");
	strcpy(res->header->connection, "close");
	// strcpy(res->header->content_encoding, "");
	strcpy(res->header->content_type, "text/html");
	strcpy(res->header->keep_alive, "close");
	strcpy(res->header->server, "C server");
	strcpy(res->header->set_cookie, "");

	strcpy(res->body, "404 Not Found");
}

char* get_response(http_response_t* res) {
	char* string_response = (char*)malloc(1024);
	
	strcpy(string_response, "");
	strcat(string_response, res->start_line->protocol);
	strcat(string_response, " ");
	char status_code[4];
	sprintf(status_code, "%d", res->start_line->status_code);
	strcat(string_response, status_code);
	strcat(string_response, " ");
	strcat(string_response, res->start_line->status_text);
	strcat(string_response, "\r\n");
	
	strcat(string_response, "Access-Control-Origin: ");
	strcat(string_response, res->header->access_control_origin);
	strcat(string_response, "\r\n");
	
	strcat(string_response, "Content-Encoding: ");
	strcat(string_response, res->header->content_encoding); 
	strcat(string_response, "\r\n");

	strcat(string_response, "Content-Type: ");
	strcat(string_response, res->header->content_type);
	strcat(string_response, "\r\n");

	strcat(string_response, "Connection: ");
	strcat(string_response, res->header->connection);
	strcat(string_response, "\r\n");

	strcat(string_response, "Keep-Alive: ");
	strcat(string_response, res->header->keep_alive);
	strcat(string_response, "\r\n");

	strcat(string_response, "Server: ");
	strcat(string_response, res->header->server);
	strcat(string_response, "\r\n");

	strcat(string_response, "Set-Cookie: ");
	strcat(string_response, res->header->set_cookie);
	strcat(string_response, "\r\n");

	strcat(string_response, "\r\n");

	strcat(string_response, res->body);	

	return string_response;
}

char* http_fn_get_response(http_request_t* req) {
	char* rs = req->start_line->request_target;

	if(strcmp(rs, "/") == 0) strcat(rs, "index.html");

	http_response_t* res = (http_response_t*)malloc(sizeof(http_response_t));
	res->start_line = (http_status_line_t*)malloc(sizeof(http_status_line_t));
	res->header = (http_response_header_t*)malloc(sizeof(http_response_header_t));

	res->start_line->protocol = (char*)malloc(sizeof(50));
	res->start_line->status_text = (char*)malloc(sizeof(100));
	res->header->access_control_origin = (char*)malloc(sizeof(50));
	res->header->connection = (char*)malloc(sizeof(50));
	res->header->content_encoding = (char*)malloc(sizeof(50));
	res->header->content_type = (char*)malloc(sizeof(50));
	res->header->keep_alive = (char*)malloc(sizeof(50));
	res->header->server = (char*)malloc(sizeof(50));
	res->header->set_cookie = (char*)malloc(sizeof(50));

	FILE* fptr = fopen(rs, "r");
	char* http_response;
	if(fptr == NULL) {
		set_error_response(res, req);
		http_response = get_response(res);
		return http_response;
	}

	char fc;
	char fcontent[2048]="";
	int i=0;
	while((fc=fgetc(fptr))!=EOF) {
		fcontent[i] = fc;
		i++;
	}

	set_response(res, req, fcontent);
	http_response = get_response(res);
	fclose(fptr);
	return http_response;

}

void http_fn_free_request(http_request_t* req) {
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

void http_fn_free_response(http_response_t* res) {
	free(res->start_line->protocol);
	free(res->start_line->status_text);
	free(res->start_line);

	free(res->header->access_control_origin);
	free(res->header->connection);
	free(res->header->content_encoding);
	free(res->header->content_type);
	free(res->header->keep_alive);
	free(res->header->server);
	free(res->header->set_cookie);
	free(res->header);
}
