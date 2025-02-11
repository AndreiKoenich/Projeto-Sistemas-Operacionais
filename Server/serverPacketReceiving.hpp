#ifndef SERVERPACKETRECEIVING_HPP
#define SERVERPACKETRECEIVING_HPP

using namespace std;

string receiveHelloPacket (int clientSocket);
void receiveRequestDownloadPacket(int clientSocket, string username);
void receiveUploadPacket (int clientSocket, string username, uint16_t packetType);
void receiveRequestListServerPacket(int clientSocket, string username);
void receiveRequestDeletePacket(int clientSocket, string username, uint16_t packetType);

#endif