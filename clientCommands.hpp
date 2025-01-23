#ifndef CLIENTCOMMANDS_HPP
#define CLIENTCOMMANDS_HPP

#include <string>
using namespace std;

void createRemoteDirectory (char username[], int clientsocket);
void helpMenu();
void showMenu(char *argv[], int clientSocket);
void uploadCommandFilename(string command, int clientSocket);
void uploadCommandPayload(string command, int clientSocket);

#endif