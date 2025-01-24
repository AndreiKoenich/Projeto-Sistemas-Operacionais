#ifndef SERVERUTILS_HPP
#define SERVERUTILS_HPP

#include "packetStruct.hpp"
using namespace std;

void createServerDirectory();
void createClientDirectory(char username[]);
void receiveUploadPacket (int clientSocket, string username);
void showUploadPacketServer (UploadPacket clientPacket);
string receiveHelloPacket (int clientSocket);
void showHelloPacketServer (HelloPacket clientPacket);

#endif
