#ifndef SERVERTHREADSTRUCT_HPP
#define SERVERTHREADSTRUCT_HPP

using namespace std;

typedef struct { 
    int clientSocket;
    int threadNumber;
    int deviceNumber = 1;
    string username = "";

} serverThreadStruct;

#endif