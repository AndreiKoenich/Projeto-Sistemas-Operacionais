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

void serverUploadPropagation (string username, UploadPacket *clientPacket, int originOfPropagationSocket) {

    clientPacket->packetType = UPLOAD_PROPAGATION;

    pthread_mutex_lock(&clientListMutex);

    list<serverThreadStruct>::iterator it;

    for (it = onlineClients.begin(); it != onlineClients.end(); it++)
        if (it->username == username && it->clientSocket != originOfPropagationSocket)
        {
            sendUploadPacket(it->clientSocket, clientPacket);
            cout << "> Foi realizada a propagacao de um arquivo para um dispositivo do usuario " << username << "." << endl;
        }
            

    pthread_mutex_unlock(&clientListMutex);
}

void serverDeletePropagation (string username, RequestDeletePacket *clientPacket, int originOfPropagationSocket) {

    clientPacket->packetType = DELETE_PROPAGATION;

    pthread_mutex_lock(&clientListMutex);

    list<serverThreadStruct>::iterator it;

    for (it = onlineClients.begin(); it != onlineClients.end(); it++)
        if (it->username == username && it->clientSocket != originOfPropagationSocket)
        {
            sendRequestDeletePacket(it->clientSocket, clientPacket);
            cout << "> Foi realizada a propagacao de uma remocao de arquivo para um dispositivo do usuario " << username << "." << endl;
        }

    pthread_mutex_unlock(&clientListMutex);
}