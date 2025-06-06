# Web server in C

## Code Example of server

To serve the `index.html` along with all the documents needed like the `styles/styles.css` to client, the following code will be able to perform it easily.

```c
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#include "http/http.h"
#include "connection/connection.h"

conn_message_t get_server_response(conn_message_t request) {
	http_request_t req = http_make_request(request->message, request->length);
	fprintf(stdout, "INFO: %s %s %s\n", req->method, req->request_target, req->protocol);

	http_response_t res = http_make_response(req);
	fprintf(stdout, "INFO: %s %d %s\n\n", res->protocol, res->status_code, res->status_text);

	char *res_str = malloc(CONNECTION_MESSAGE_MAX_LEN);
	http_get_response_string(res, res_str, CONNECTION_MESSAGE_MAX_LEN);

	static struct conn_message conn_res;
	strcpy(conn_res.message, res_str);
	conn_res.length = strlen(res_str);
	
	free(req);
	free(res);
	free(res_str);
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
	free(server);
	return 0; 
}
```

## TO DO

- Support multiple formats of response - image, application etc.
- Support dynamic sites where user can submit data and it will save in the database.
- Support multi-client server where multiple clients request the server and it can serve them all at the same time without blocking any request.

## Resources:

1. https://www.beej.us/guide/bgnet/html/split/index.html
2. https://github.com/bilalmohib/ProgrammingBooks/blob/crystal/hands-on-network-programming-with-c-learn-socket-programming-in-c-and-write-secure-and-optimized-network-code-true-pdf-1nbsped-9781789349863_compress.pdf
