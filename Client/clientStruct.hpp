#ifndef CLIENTSTRUCT_HPP
#define CLIENTSTRUCT_HPP

#include <stdint.h>

using namespace std;

typedef struct { 
    int clientSocket;
    char* username;
    char* address;
    char* port;
      
} clientStruct;

#endif