#ifndef SERVERPACKETPROPAGATION_HPP
#define SERVERPACKETPROPAGATION_HPP

#include <string>
#include "packetStruct.hpp"

void serverUploadPropagation (string username, UploadPacket *clientPacket, int originOfPropagationSocket);
void serverDeletePropagation (string username, RequestDeletePacket *clientPacket, int originOfPropagationSocket);

#endif
