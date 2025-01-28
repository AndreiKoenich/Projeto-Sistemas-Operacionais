#ifndef CLIENTUTILS_HPP
#define CLIENTUTILS_HPP

#include "packetStruct.hpp"

int getch_();
void reverseString(char* str);

void showUploadPacketClient (UploadPacket clientPacket);
void showHelloPacketClient (HelloPacket clientPacket);
void showByePacketClient (ByePacket clientPacket);

void createRemoteDirectory (char username[], int clientSocket);
void createClientDirectory(char username[]);

void showMenu(char *argv[], int clientSocket);
void helpMenu();

#endif