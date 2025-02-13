#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <list>
#include <iterator>
#include <iostream>

#include "serverConstants.hpp"
#include "packetEnum.hpp"
#include "serverUtils.hpp"
#include "serverPacketReceiving.hpp"
#include "serverThreadStruct.hpp"

using namespace std;

pthread_t *clientThreads;

list<serverThreadStruct> onlineClients;
pthread_mutex_t clientListMutex = PTHREAD_MUTEX_INITIALIZER;

void addNewClientInList (serverThreadStruct *clientInfo) {

    pthread_mutex_lock(&clientListMutex);

    //list<serverThreadStruct>::iterator it;

    //for (it = onlineClients.begin(); it != onlineClients.end(); it++)
      //  if (it->username == clientInfo->username)
            //clientInfo->deviceNumber++;

    onlineClients.push_back(*clientInfo);
    cout << "> Novo dispositivo do usuario " << clientInfo->username << " foi conectado." << endl;

    pthread_mutex_unlock(&clientListMutex);
}

void removeClientFromList (serverThreadStruct *clientInfo) {

    pthread_mutex_lock(&clientListMutex);

    list<serverThreadStruct>::iterator it;

    for (it = onlineClients.begin(); it != onlineClients.end(); it++)
        if (it->username == clientInfo->username && it->clientSocket == clientInfo->clientSocket) {
            it = onlineClients.erase(it);
            break;
        }

    close(clientInfo->clientSocket);

    cout << "> Dispositivo do usuario " << clientInfo->username << " foi desconectado." << endl;
            
    pthread_mutex_unlock(&clientListMutex);

    pthread_cancel(clientThreads[clientInfo->threadNumber-1]);

}

void* receivePacketFromClient (void* threadInfo) {

    serverThreadStruct clientInfo;
    clientInfo.clientSocket = ((serverThreadStruct*) threadInfo)->clientSocket;
    clientInfo.threadNumber = ((serverThreadStruct*) threadInfo)->threadNumber;
    delete (serverThreadStruct*) threadInfo;

    uint16_t packetType;

    while (true) {

        recv(clientInfo.clientSocket, &packetType, sizeof(packetType), 0);
        packetType = ntohs(packetType);

        switch (packetType) {
            case HELLO:
                clientInfo.username = receiveHelloPacket(clientInfo.clientSocket);
                addNewClientInList(&clientInfo);
            break;
            case REQUEST_DOWNLOAD:
                receiveRequestDownloadPacket(clientInfo.clientSocket, clientInfo.username);
            break;
            case UPLOAD:
                receiveUploadPacket(clientInfo.clientSocket, clientInfo.username, packetType);
            break;
            case UPLOAD_INOTIFY:
                receiveUploadPacket(clientInfo.clientSocket, clientInfo.username, packetType);
            break;
            case REQUEST_LIST_SERVER:
                receiveRequestListServerPacket(clientInfo.clientSocket, clientInfo.username);
            break;
            case DELETE_INOTIFY:
                receiveRequestDeletePacket(clientInfo.clientSocket, clientInfo.username, packetType);
            break;
            case BYE_CLIENT:
                removeClientFromList(&clientInfo);
            break;
        }
    }

    return NULL;
}

void serverLoop (int serverSocket) {

    int threadNumber = 0;
    clientThreads = (pthread_t*) malloc(sizeof(pthread_t));

    while (true) {

        int clientSocket = accept(serverSocket, nullptr, nullptr);
        
        threadNumber++;

        clientThreads = (pthread_t*) realloc(clientThreads,threadNumber*sizeof(pthread_t));

        serverThreadStruct threadStruct;
        threadStruct.clientSocket = clientSocket;
        threadStruct.threadNumber = threadNumber;

        serverThreadStruct *threadStructPtr = new serverThreadStruct (threadStruct);

        pthread_create(&clientThreads[threadNumber-1], NULL, receivePacketFromClient, (void*) threadStructPtr);
    }
}

void startServerSocket() {
    int serverSocket = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT_NUMBER);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(serverSocket,(struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket,MAX_SERVER_QUEUE_SIZE);

    system("clear");
    cout << "PROJETO DE SISTEMAS OPERACIONAIS II" << endl;
    cout << "AUTOR: ANDREI POCHMANN KOENICH\n" << endl;
    cout << "-----------------------------------------------\n" << endl;

    cout << "Servidor escutando na porta " << SERVER_PORT_NUMBER << "..." << endl;
    serverLoop(serverSocket);
    close(serverSocket);
}

int main(int argc, char *argv[]) {
    createServerDirectory();
    startServerSocket();
    return 0;
}