#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <stdbool.h>
#include <stdlib.h>

/* Constants */
#define PORT 80
#define HASH_TABLE_SIZE 256
#define FNV_OFFSET_BASIS 2166136261
#define FNV_PRIME 16777619

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
    struct HashTable* headers[HASH_TABLE_SIZE];

    char body[1024];
} typedef Request;

struct {

} typedef Response;

/* Function declarations */

unsigned int CalcHash(const char* key);
Request* NewRequest(int socket);
char* ProcessRequest(Request*);
struct HashEntry* createEntry(char*, char*);
struct HashTable* createHashTable();
void insert(struct HashTable*, char*, char*);
char* search(struct HashTable*,char*);

#endif