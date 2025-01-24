#ifndef SERVERUTILS_HPP
#define SERVERUTILS_HPP

#include "packetStruct.hpp"
using namespace std;

void createServerDirectory();
void createClientDirectory(char username[]);
void receiveUploadPacket (int clientSocket);
void showUploadPacketServer (UploadPacket clientPacket);
void receiveHelloPacket (int clientSocket);
void showHelloPacketServer (HelloPacket clientPacket);

#endif
