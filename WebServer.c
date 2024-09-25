#include "WebServer.h"

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
        Request* rq;
        if(rq = NewRequest(new_socket))
        {
            Response* rp = ProcessRequest(rq);
        }
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

    Request* rq = mallac(sizeof(Request));
    char* line;
    char* end_ln;
    int lineCount = 0;
    bool headerDone = false;

    char* line = strtok_r(buffer, "\n", &end_ln);
    while(line != NULL){
        int wordCount = 0;

        if(lineCount == 0)
        {
            //Start line here
            char* end_wd;
            char* word = strtok_r(line, " ", &end_wd);

            while(word != NULL)
            {
                word = strtok_r(NULL, " ", &end_wd);
                if(wordCount == 0)
                {
                    switch (word)
                    {
                    case "CONNECT":
                        rq->methodToken = CONNECT;
                        break;
                    case "DELETE":
                        rq->methodToken = DELETE;
                        break;
                    case "GET":
                        rq->methodToken = GET;
                        break;
                    case "HEAD":
                        rq->methodToken = HEAD;
                        break;
                    case "OPTIONS":
                        rq->methodToken = OPTIONS;
                        break;
                    case "PATCH":
                        rq->methodToken = PATCH;
                        break;
                    case "POST":
                        rq->methodToken = POST;
                        break;
                    case "PUT":
                        rq->methodToken = PUT;
                        break;
                    case "TRACE":
                        rq->methodToken = TRACE;
                        break;
                    default:
                        rq->methodToken = INVALID;
                        break;
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
                    NewResponse(socket, )
                }
                wordCount++;
            }
            line = stdtok_r(NULL, "\n", &end_ln);
            lineCount++;
        }
        else if(lineCount > 0 && line != "" && !(headerDone))
        {
            //Header fields here
            char* end_wd;
            char* word = strtok_r(line, " ", &end_wd);

            while(word != NULL)
            {
                word = strtok_r(NULL, " ", &end_wd);
                wordCount++;
            }
            line = stdtok_r(NULL, "\n", &end_ln);
            lineCount++;
        }
        else
        {
            headerDone = true;
        }

        //Decide if need to read body and do so here
        char* end_wd;
        char* word = strtok_r(line, " ", &end_wd);

        while(word != NULL)
        {
            word = strtok_r(NULL, " ", &end_wd);
            wordCount++;
        }
        line = stdtok_r(NULL, "\n", &end_ln);
        lineCount++;
    }

    return 0;
}

char* GetValue(char* key)
{
    unsigned int index = CalcHash(key);
    
    return hashMap[index];
}

char* GetValueRecursive(char* key)
{

}

void InsertValue(char* key, char* value)
{
    unsigned int index = CalcHash(key);
    HashEntry he;

    memcpy(he.key, key);
    strcpy(he.value, value);

    if(hashMap[index].key != "")
    {
        ResolveCollision(he, key);
    }else
    {
        hashMap[index] = he;
    }
}

HashEntry* ResolveCollision(HashEntry he, char* key)
{
    if(he.node != NULL)
    {
        if(he.key == key)
        {   
            return he.node;
        }
        FindLast(he.node);
    }

    return he.node;
}

unsigned int CalcHash(const char* str)
{
    unsigned int hash = FNV_OFFSET_BASIS;
    
    for(int i = 0; i < 256; i++)
    {
        hash = hash * FNV_PRIME;
        hash = hash ^ str[i];
    }
    return hash%HASH_TABLE_SIZE;
}