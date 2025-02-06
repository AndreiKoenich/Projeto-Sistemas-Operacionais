#ifndef INOTIFYCLIENT_HPP
#define INOTIFYCLIENT_HPP

#include "clientStruct.hpp"

void uploadInotify(string fileName, clientStruct *menuParameters);
void deleteInotify(string fileName, clientStruct *menuParameters);
bool isTemporaryFile(const string filename);
void* monitorClientDirectory (void* menuParameters);

#endif