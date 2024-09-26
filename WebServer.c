#include "WebServer.h"

#include <asm-generic/socket.h>
#include <complex.h>
#include <search.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <signal.h>

void signal_handler(int signum)
{
    running = false;
}

int main()
{
    signal(SIGINT, signal_handler);
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
        Request* rq = NewRequest(new_socket);
        char* rp = ProcessRequest(rq);
        send(new_socket, rp, strlen(rp), 0);
    }

    valread = read(new_socket, buffer, 1024-1);

    printf("%s\n", buffer);
    //send(new_socket, hello, strlen(hello), 0);
    //printf("Hello message sent\n");

    close(new_socket);
    close(server_fd);
    return 0;
}

Request* NewRequest(int socket)
{
    char buffer[1024];
    int val = recv(socket, buffer, sizeof(buffer), 0);

    Request* rq = malloc(sizeof(Request));
    //hcreate_r(256, rq->headers);
    char* line;
    char* end_ln;
    int lineCount = 0;
    bool headerDone = false;

    line = strtok_r(buffer, "\r", &end_ln);
    while(line != NULL){
        int wordCount = 0;

        if(lineCount == 0)
        {
            //Start line here
            char* end_wd;
            char* word = strtok_r(line, " ", &end_wd);

            while(word != NULL)
            {
                if(wordCount == 0)
                {
                    if(!(strcmp(word, "CONNECT")))
                    {
                        rq->methodToken = CONNECT;
                    }
                    else if(!(strcmp(word, "DELETE")))
                    {
                        rq->methodToken = DELETE;
                    }
                    else if(!(strcmp(word, "GET")))
                    {
                        rq->methodToken = GET;
                    }
                    else if(!(strcmp(word, "HEAD")))
                    {
                        rq->methodToken = HEAD;
                    }
                    else if(!(strcmp(word, "OPTIONS")))
                    {
                        rq->methodToken = OPTIONS;
                    }
                    else if(!(strcmp(word, "PATCH")))
                    {
                        rq->methodToken = PATCH;
                    }
                    else if(!(strcmp(word, "POST")))
                    {
                        rq->methodToken = POST;
                    }
                    else if(!(strcmp(word, "PUT")))
                    {
                        rq->methodToken = PUT;
                    }
                    else
                    {
                        rq->methodToken = INVALID;
                    }
                }

                if(wordCount == 1)
                {
                    rq->requestTarget = word;
                }

                if(wordCount == 2)
                {
                    rq->protocolVersion = word;
                }

                if(wordCount > 2)
                {
                    //NewResponse(socket, )
                }
                word = strtok_r(NULL, " ", &end_wd);
                wordCount++;
            }
        }
        else if(lineCount > 0 && !(headerDone))
        {
            //Header fields here
            strcat(rq->headers, line);
        }
        else if(lineCount > 0 && !(strcmp(line, "\n")))
        {
            headerDone = true;
            continue;
        }
        else //Do we need to read body?
        {
            strcat(rq->body, line);
        }
        line = strtok_r(NULL, "\n", &end_ln);
        lineCount++;
    }

    return rq;
}

char* ProcessRequest(Request* rq)
{
    if(rq->methodToken == GET)
    {
        printf("GET %s %s\n", rq->requestTarget, rq->protocolVersion);
        printf("%s", rq->headers);
        printf("%s", rq->body);
    }

    char* content = malloc(1024);
    char buff[1024];

    char* header = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\nDate: Wed, 25 Sep 2024 14:18:33 GMT\nLast-Modified: Thu, 17, Oct 2019 07:18:26 GMT\nContent-Length: 1024\n\n";

    strcat(content, header);

    FILE* fd = fopen("srv/index.html", "r");

    fread(buff, sizeof(char), 1024, fd);

    strcat(content, buff);

    return content;
}

char* GetValue(struct HashEntry he[], const char* k)
{
    unsigned int index = CalcHash(k);
    if(he[index]->key != k)
    {
        //Traverse linked list
    }

    return he[index]->value;
}

void SetKey(struct HashEntry he[], const char* k, const char* v)
{
    unsigned int index = CalcHash(k)
    if(he[index] == NULL)
    {
        he[index]->key = k;
        he[index]->value = v;
    }
    else if(he[index]->key == k){
        he[index]->value = v;
    }
    else
    {
        Traverse(he, k, v);
    }
}

struct HashEntry* Traverse(struct HashEntry* he, const char* k, const char* v)
{
    if(he->key != k)
    {
        Traverse(he->link, k, v);
    }
    return he;
}

unsigned int CalcHash(const char* key)
{
    unsigned int hash = FNV_OFFSET_BASIS;

    for (int i = 0; i < strlen(key); i++)
    {
        hash = hash * FNV_PRIME;
        hash = hash ^ char[i];
    }

    return hash%256;
}