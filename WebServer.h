#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <stdbool.h>

#define PORT 80
#define HASH_TABLE_SIZE 256
#define FNV_OFFSET_BASIS 2166136261
#define FNV_PRIME 16777619

struct HashEntry{
    char key[128];
    char value[128];
    struct HashEntry* node;
};

bool running;
struct HashEntry hashMap[HASH_TABLE_SIZE];

enum {
    INVALID,
    CONNECT,
    DELETE,
    GET,
    HEAD,
    OPTIONS,
    PATCH,
    POST,
    PUT,
    TRACE
} typedef Method;

struct {
    Method methodToken;
    char* requestTarget;
    char* protocolVersion;

    char hashMap[256][50];

    char* body;
} typedef Request;

struct {

} typedef Response;


unsigned int CalcHash(const char* str);
void InsertValue(char*,char*);
char* GetValue(char* key);
Request* NewRequest(int socket);
Response* ProcessRequest(Request*);
struct HashEntry* ResolveCollision(struct HashEntry, char*);

#endif