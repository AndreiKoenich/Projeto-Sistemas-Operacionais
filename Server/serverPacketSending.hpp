#ifndef SERVERPACKETSENDING_HPP
#define SERVERPACKETSENDING_HPP

#include "packetStruct.hpp"

void sendDownloadPacket(int clientSocket, DownloadPacket *clientPacket);

#endif
