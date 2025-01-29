#ifndef CLIENTCOMMANDS_HPP
#define CLIENTCOMMANDS_HPP

#include <string>
using namespace std;

void createRemoteDirectory (char username[], int clientsocket);
void helpMenu();
void showMenu(char *argv[], int clientSocket);
void requestDownloadCommand(string username, string command, int clientSocket);
void uploadCommand(string command, int clientSocket);
void listClientCommand(string username);
void requestListServerCommand(string username, int clientSocket);
void exitCommand(int clientSocket);
void deleteCommand (string username, string command);

#endif