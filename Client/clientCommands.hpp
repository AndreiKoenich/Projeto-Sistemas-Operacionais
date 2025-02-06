#ifndef CLIENTCOMMANDS_HPP
#define CLIENTCOMMANDS_HPP

#include "clientStruct.hpp"

using namespace std;

void createRemoteDirectory (char username[], int clientsocket);
void helpMenu();
void requestDownloadCommand(string username, string command, clientStruct *menuParameters);
void uploadCommand(string command, int clientSocket);
void listClientCommand(string username);
void requestListServerCommand(string username, int clientSocket);
void exitCommand(int clientSocket);
void deleteCommand (string username, string command);
void deleteInotify(string username, string fileName, int clientSocket);

#endif