#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#include "http/http.h"
#include "connection/connection.h"

conn_message_t get_server_response(conn_message_t request) {
	http_request_t req = http_make_request(request->message, request->length);
	http_response_t res = http_make_response(req);
	char *res_str = http_get_response_string(res);;

	static struct conn_message conn_res;
	strcpy(conn_res.message, res_str);
	conn_res.length = strlen(res_str);

	return &conn_res;
}

int main(int argc, char* argv[]){
	if(argc < 3) {
		printf("Format: ./server 127.0.0.1 6666\n");
		return 1;
	}

	char *address = argv[1];
	int port = atoi(argv[2]);
	
	conn_host_t server = conn_get_host(address, port);
	conn_handle_client_request(server, get_server_response);

	return 0; 
}

