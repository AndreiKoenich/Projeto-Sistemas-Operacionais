#ifndef PACKETSTRUCT_HPP
#define PACKETSTRUCT_HPP

#include <stdint.h>

using namespace std;

typedef struct { 
    uint16_t type;
    uint16_t payloadLength;
    char* payload;    
} Packet; 

#endif