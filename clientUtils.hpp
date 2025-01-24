#ifndef CLIENTUTILS_HPP
#define CLIENTUTILS_HPP

#include "packetStruct.hpp"

int getch_();
void reverseString(char* str);

void sendUploadPacket(int clientSocket, UploadPacket *clientPacket);
void showUploadPacketClient (UploadPacket clientPacket);

void sendHelloPacket(int clientSocket, HelloPacket *clientPacket);
void showHelloPacketClient (HelloPacket clientPacket);

void createRemoteDirectory (char username[], int clientSocket);
void createClientDirectory(char username[]);

void showMenu(char *argv[], int clientSocket);
void helpMenu();

#endif