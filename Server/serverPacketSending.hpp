#ifndef SERVERPACKETSENDING_HPP
#define SERVERPACKETSENDING_HPP

void sendUploadPacket(int clientSocket, UploadPacket *clientPacket);
void sendDownloadPacket(int clientSocket, DownloadPacket *clientPacket);
void sendDownloadErrorPacket(int clientSocket, DownloadErrorPacket *clientPacket);
void sendListServerPacket(int clientSocket, ListServerPacket *clientPacket);
void sendRequestDeletePacket(int clientSocket, RequestDeletePacket *clientPacket);

#endif
