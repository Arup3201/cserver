# Web Server Implementation using C

What are the things I need?
1. Build a socket for server
2. Connect to clients
3. Send HTTP response

## Build a socket server

To build a socket server, I need `sys/socket.h` library.

I need the `socket` function to create the socket server.

## Connect to clients


## Send HTTP response

NOTE:

Some important points to note here -
1. Send the response with new lines denoted as `\r\n`.
2. The response should contain `Contain-Type` as `text/html` or some other compatible type so that the browser don't download a file, instead show a the content in the browser page.
3. Do not `close` the connection, instead `shutdown` the connection so that the browser does not keep on loading.
