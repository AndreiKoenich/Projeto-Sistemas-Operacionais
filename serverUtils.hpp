#ifndef SERVERUTILS_HPP
#define SERVERUTILS_HPP

#include "packetStruct.hpp"
using namespace std;

void createServerDirectory();
void createClientDirectory(char username[]);
void showPacketServer (Packet clientPacket);

#endif
