// source: https://devtails.xyz/@adam/how-to-build-a-simple-tcp-server-in-c
#include <stdio.h>
#include <netinet/in.h> // sockaddr_in, INADDR_ANY
#include <sys/socket.h> // socket, bind, listen, accept, AF_INET, SOCK_STREAM
#include <sys/types.h>  // htonl, htons, socklen_t
#include <unistd.h>     // read, write, close
#include <sys/stat.h>
// This is used to pre-allocate the char array size
// For our use case, we will accept that it's not possible to read messages larger than this
#define MAX_MESSAGE_SIZE 256

int main(void)
{
    // Initialize the details of the server socket
    struct sockaddr_in server_sockaddr_in;

    // Define socket family AF_INET = internetwork: UDP, TCP, etc.
    server_sockaddr_in.sin_family = AF_INET;

    // https://linux.die.net/man/3/htonl
    // The htonl() function converts the unsigned integer hostlong from host byte order to network byte order.
    server_sockaddr_in.sin_addr.s_addr = htonl(INADDR_ANY);

    const int port = 5000;
    // The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.
    server_sockaddr_in.sin_port = htons(port);

    // https://man7.org/linux/man-pages/man2/socket.2.html
    // creates an endpoint for communication and returns a file descriptor that refers to that endpoint
    // SOCK_STREAM defines that this should communicate over TCP
    int socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    struct stat st;

    fstat(socket_file_descriptor, &st);
    printf("Socket created with inode: %lu\n", st.st_ino);

    // https://man7.org/linux/man-pages/man2/bind.2.html
    // bind() assigns the address specified by server_sockaddr_in to the socket socket_file_descriptor
    bind(socket_file_descriptor, (struct sockaddr *)&server_sockaddr_in, sizeof(server_sockaddr_in));

    // https://man7.org/linux/man-pages/man2/listen.2.html
    // listen() marks the socket referred to by sockfd as a passive socket
    // the second parameter (5) defines the maximum length to which the queue of pending connections for socket_file_descriptor may grow
    listen(socket_file_descriptor, 5);

    struct sockaddr_in client_sockaddr_in;
    socklen_t len = sizeof(client_sockaddr_in);

    // https://man7.org/linux/man-pages/man2/accept4.2.html
    // accept() extracts the first connection request on the queue of pending connections for the listening socket
    // The address info from the client will be stored in client_sockaddr_in
    int connection_file_descriptor = accept(socket_file_descriptor, (struct sockaddr *)&client_sockaddr_in, &len);

    char buffer[MAX_MESSAGE_SIZE] = {};

    // https://man7.org/linux/man-pages/man2/read.2.html
    // read() attempts to read up to MAX_MESSAGE_SIZE bytes from file descriptor connection_file_descriptor into buffer
    read(connection_file_descriptor, buffer, sizeof(buffer));
    printf("%s", buffer);

    char status = 0;
    // https://man7.org/linux/man-pages/man2/write.2.html
    // write(fd, buf, count) writes up to count bytes from the buffer starting at buf to the file referred to by the file descriptor fd
    // In this case we simply send a single byte 0 to indicate we have received the message
    write(connection_file_descriptor, &status, 1);

    // https://man7.org/linux/man-pages/man2/close.2.html
    // close() closes a file descriptor, so that it no longer refers to any file and may be reused
    close(socket_file_descriptor);

    return 0;
}