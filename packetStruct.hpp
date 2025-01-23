#ifndef PACKETSTRUCT_HPP
#define PACKETSTRUCT_HPP

#include <stdint.h>

using namespace std;

typedef struct { 
    uint16_t type;
    
    uint16_t fileNameLength;
    char * fileName;

    uint16_t payloadLength;
    char* payload; 
      
} UploadPacket; 

typedef struct { 
    uint16_t type;

    uint16_t usernameLength;
    char* username; 
      
} HelloPacket; 

#endif