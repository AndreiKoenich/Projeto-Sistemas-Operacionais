#ifndef SERVERTHREADSTRUCT_HPP
#define SERVERTHREADSTRUCT_HPP

using namespace std;

typedef struct { 
    int clientSocket;
    int threadNumber;
    string username = "";

} serverThreadStruct;

#endif