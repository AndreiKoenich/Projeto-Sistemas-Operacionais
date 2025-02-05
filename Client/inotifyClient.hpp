#ifndef INOTIFYCLIENT_HPP
#define INOTIFYCLIENT_HPP

#include "clientStruct.hpp"

void deleteInotify(string fileName, int clientSocket);
void uploadInotify(string fileName, string username, int clientSocket);
bool isTemporaryFile(const string filename);
void* monitorClientDirectory (void* menuParameters);

#endif