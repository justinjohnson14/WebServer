#include <asm-generic/socket.h>
#include <bits/pthreadtypes.h>
#include <complex.h>
#include <sys/socket.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <stdbool.h>

#define PORT 80

pthread_t connectionThread;
bool running;

void* newConnection(int socket);

struct ConnectionArgs{
    
};

int main()
{
    running = true;
    int server_fd, new_socket;
    ssize_t valread;

    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0};
    char* hello = "Hello from server";

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("failed to set socket options");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 3) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    while(running)
    {
        if((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0)
        {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }
        pthread_create(&connectionThread, NULL, (void*)newConnection, &new_socket);
    }

    valread = read(new_socket, buffer, 1024-1);

    printf("%s\n", buffer);
    //send(new_socket, hello, strlen(hello), 0);
    //printf("Hello message sent\n");

    close(new_socket);
    close(server_fd);
    return 0;
}

void* newConnection(int socket)
{

}