#ifndef CLIENTPACKETRECEIVING_HPP
#define CLIENTPACKETRECEIVING_HPP

using namespace std;

void receiveDownloadPacket(int clientSocket, string username);
void receivePacketFromServer (int clientSocket, string username);
void receiveListServerPacket(int clientSocket);

#endif