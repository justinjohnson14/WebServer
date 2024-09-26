#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <stdbool.h>

/* Constants */
#define PORT 80
#define HASH_TABLE_SIZE 256
#define FNV_OFFSET_BASIS 2166136261
#define FNV_PRIME 16777619

typedef struct HashEntry;

bool running;

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

struct HashEntry{
    char* key = NULL;
    char* val = NULL;
    struct HashEntry* link = NULL;
};

struct HashTable {
    struct HashEntry** table;
};

struct {
    Method methodToken;
    char* requestTarget;
    char* protocolVersion;

    //struct hsearch_data* headers;
    struct HashEntry headers[HASH_TABLE_SIZE];

    char body[1024];
} typedef Request;

struct {

} typedef Response;

/* Function declarations */

unsigned int CalcHash(const char* key);
void InsertValue(char*,char*);
char* GetValue(char* key);
Request* NewRequest(int socket);
char* ProcessRequest(Request*);
struct HashEntry* ResolveCollision(struct HashEntry, char*);

#endif