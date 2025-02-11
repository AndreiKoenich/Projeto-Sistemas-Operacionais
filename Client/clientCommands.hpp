#ifndef CLIENTCOMMANDS_HPP
#define CLIENTCOMMANDS_HPP

#include "clientStruct.hpp"

using namespace std;

void createRemoteDirectory (char username[], int clientsocket);
void helpMenu();
void requestDownloadCommand(string command, clientStruct *menuParameters);
void uploadCommand(string command, clientStruct *menuParameters);
void listClientCommand(clientStruct *menuParameters);
void requestListServerCommand(clientStruct *menuParameters);
void deleteCommand (string command, clientStruct *menuParameters);
void deleteInotify(string username, string fileName, int clientSocket);

#endif