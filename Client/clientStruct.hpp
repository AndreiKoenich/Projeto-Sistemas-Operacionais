#ifndef CLIENTSTRUCT_HPP
#define CLIENTSTRUCT_HPP

using namespace std;

typedef struct { 
    int clientSocket;
    char* username;
    char* address;
    char* port;
      
} clientStruct;

#endif