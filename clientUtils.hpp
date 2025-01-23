#ifndef CLIENTUTILS_HPP
#define CLIENTUTILS_HPP

#include "packetStruct.hpp"

int getch_();
void reverseString(char* str);
void showPacketClient (UploadPacket clientPacket);
void sendPacket(int clientSocket, UploadPacket *clientPacket);
void createRemoteDirectory (char username[], int clientSocket);
void createClientDirectory(char username[]);
void showMenu(char *argv[], int clientSocket);
void helpMenu();

#endif