#ifndef CLIENTUTILS_HPP
#define CLIENTUTILS_HPP

#include "packetStruct.hpp"

void reverseString(char* str);
string timeToString(time_t rawTime);
string showPacketType(int value);

void showRequestDownloadPacketClient(RequestDownloadPacket clientPacket);
void showDownloadPacketClient (DownloadPacket clientPacket);
void showUploadPacketClient (UploadPacket clientPacket);
void showHelloPacketClient (HelloPacket clientPacket);
void showByePacketClient (ByePacket clientPacket);
void showRequestListServerPacketClient(RequestListServerPacket clientPacket);
void showListServerPacketClient(ListServerPacket clientPacket);
void showRequestDeletePacketClient(RequestDeletePacket clientPacket);
void showReceivedPropagationPacketClient (UploadPacket clientPacket);

void createRemoteDirectory (char username[], int clientSocket);
void createClientDirectory(char username[]);

void helpMenu();

#endif