#include <arpa/inet.h>
#include <bits/stdc++.h>

#include "serverConstants.hpp"
#include "packetEnum.hpp"
#include "serverUtils.hpp"
#include "serverPacketReceiving.hpp"
#include "serverThreadStruct.hpp"

using namespace std;

pthread_t *clientThreads;

void* receivePacketFromClient (void* threadInfo) {

    int clientSocket = ((serverThreadStruct*) threadInfo)->clientSocket;
    int threadNumber = ((serverThreadStruct*) threadInfo)->threadNumber;
    delete (serverThreadStruct*) threadInfo;

    string username("");
    uint16_t packetType;

    while (true) {

        recv(clientSocket, &packetType, sizeof(packetType), 0);
        packetType = ntohs(packetType);

        switch (packetType) {
            case HELLO:
                username = receiveHelloPacket(clientSocket);
            break;
            case REQUEST_DOWNLOAD:
                receiveRequestDownloadPacket(clientSocket, username);
            break;
            case UPLOAD:
                receiveUploadPacket(clientSocket, username);
            break;
            case UPLOAD_INOTIFY:
                receiveUploadPacket(clientSocket, username);
            break;
            case REQUEST_LIST_SERVER:
                receiveRequestListServerPacket(clientSocket, username);
            break;
            case DELETE_INOTIFY:
                receiveRequestDeletePacket(clientSocket, username);
            break;
            case BYE:
                //showByePacketServer(username);
                close(clientSocket);
                pthread_cancel(clientThreads[threadNumber-1]);
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

        system("clear");
        threadNumber++;

        cout << "Servidor escutando na porta " << SERVER_PORT_NUMBER << "..." << endl;
        cout << "Nova conexao aceita com sucesso. Numero de conexoes aceitas ate o momento: " << threadNumber << endl;

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
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(serverSocket,(struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket,MAX_SERVER_QUEUE_SIZE);

    cout << "Servidor escutando na porta " << SERVER_PORT_NUMBER << "..." << endl;
    serverLoop(serverSocket);
    close(serverSocket);
}

int main(int argc, char *argv[]) {
    createServerDirectory();
    startServerSocket();
    return 0;
}