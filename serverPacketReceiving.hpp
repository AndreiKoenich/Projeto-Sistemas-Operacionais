#ifndef SERVERPACKETRECEIVING_HPP
#define SERVERPACKETRECEIVING_HPP

using namespace std;

string receiveHelloPacket (int clientSocket);
void receiveUploadPacket (int clientSocket, string username);

#endif