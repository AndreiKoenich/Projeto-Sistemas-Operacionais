#ifndef SERVERPACKETSENDING_HPP
#define SERVERPACKETSENDING_HPP

#include "packetStruct.hpp"

void sendDownloadPacket(int clientSocket, DownloadPacket *clientPacket);
void sendDownloadErrorPacket(int clientSocket, DownloadErrorPacket *clientPacket);
void sendListServerPacket(int clientSocket, ListServerPacket *clientPacket);

#endif
