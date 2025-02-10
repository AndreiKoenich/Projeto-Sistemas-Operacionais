#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <list>
#include <iterator>
#include <iostream>

#include "packetStruct.hpp"
#include "serverConstants.hpp"
#include "packetEnum.hpp"
#include "serverUtils.hpp"
#include "serverPacketReceiving.hpp"
#include "serverPacketSending.hpp"
#include "serverThreadStruct.hpp"

using namespace std;

extern list<serverThreadStruct> onlineClients;
extern pthread_mutex_t clientListMutex;
extern int serverSocket; 

void serverUploadPropagation (string username, UploadPacket *clientPacket) {

    clientPacket->packetType = UPLOAD_PROPAGATION;

    list<serverThreadStruct>::iterator it;

    for (it = onlineClients.begin(); it != onlineClients.end(); it++)
        if (it->username == username)
            sendUploadPacket(it->clientSocket, clientPacket);
}

void serverDeletePropagation (string username, RequestDeletePacket *clientPacket) {

    clientPacket->packetType = DELETE_PROPAGATION;

    list<serverThreadStruct>::iterator it;

    for (it = onlineClients.begin(); it != onlineClients.end(); it++)
        if (it->username == username)
            sendRequestDeletePacket(it->clientSocket, clientPacket);
    
}