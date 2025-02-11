#ifndef CLIENTPACKETRECEIVING_HPP
#define CLIENTPACKETRECEIVING_HPP

using namespace std;

void receiveDownloadPacket(int clientSocket, string username);
void receiveListServerPacket(int clientSocket);
void receiveUploadPropagationPacket(int clientSocket, string username);
void receiveDeletePropagationPacket(int clientSocket, string username);
void* receivePacketFromServerLoop (void* parameters);

#endif