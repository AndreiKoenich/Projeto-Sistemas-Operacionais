#ifndef PACKETSTRUCT_HPP
#define PACKETSTRUCT_HPP

#include <stdint.h>

using namespace std;

typedef struct { 
    uint16_t packetType;
    
    uint16_t fileNameLength;
    char * fileName;
      
} RequestDownloadPacket; 

typedef struct { 
    uint16_t packetType;
    
    uint16_t fileNameLength;
    char * fileName;

    uint16_t payloadLength;
    char* payload; 
      
} DownloadPacket;

typedef struct { 
    uint16_t packetType;
      
} DownloadErrorPacket; 

typedef struct { 
    uint16_t packetType;
    
    uint16_t fileNameLength;
    char * fileName;

    uint16_t payloadLength;
    char* payload; 
      
} UploadPacket; 

typedef struct { 
    uint16_t packetType;
      
} RequestListServerPacket; 

typedef struct { 
    uint16_t packetType;

    uint16_t payloadLength;
    char* payload; 
      
} ListServerPacket; 

typedef struct { 
    uint16_t packetType;

    uint16_t usernameLength;
    char* username; 
      
} HelloPacket; 

typedef struct { 
    uint16_t packetType;
      
} ByePacket; 

#endif