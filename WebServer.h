#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#define PORT 80
#define HASH_TABLE_SIZE 256

bool running;
HashEntry hashMap[HASH_TABLE_SIZE];

struct {
    char key[128];
    char value[128];
    HashEntry* node;
}typedef HashEntry;

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

#endif