#ifndef CLIENTUTILS_HPP
#define CLIENTUTILS_HPP

#include "packetStruct.hpp"

int getch_();
void reverseString(char* str);
void showPacketClient (Packet clientPacket);
void sendPacket(int clientSocket, Packet *clientPacket);
void createRemoteDirectory (char username[], int clientSocket);
void createClientDirectory(char username[]);
void showMenu(char *argv[], int clientSocket);
void helpMenu();

#endif