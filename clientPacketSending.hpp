#ifndef CLIENTPACKETSENDING_HPP
#define CLIENTPACKETSENDING_HPP

#include "packetStruct.hpp"

void sendUploadPacket(int clientSocket, UploadPacket *clientPacket);
void sendHelloPacket(int clientSocket, HelloPacket *clientPacket);
void sendByePacket(int clientSocket, ByePacket *clientPacket);

#endif
