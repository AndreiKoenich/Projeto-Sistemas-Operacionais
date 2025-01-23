#include <iostream>
#include <sys/socket.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>

#include "serverConstants.hpp"
#include "packetStruct.hpp"
#include "packetEnum.hpp"
#include "serverUtils.hpp"

using namespace std;

void receivePacket (int serverSocket) {

    int clientSocket = accept(serverSocket, nullptr, nullptr);
    cout << "Nova conexao aceita com sucesso." << endl;

    UploadPacket clientPacket;
    uint16_t type;
    uint16_t payloadLength;
    uint16_t fileNameLength;

    //while (true) {
        recv(clientSocket, &type, sizeof(type), 0);
        clientPacket.type = ntohs(type);

        recv(clientSocket, &fileNameLength, sizeof(payloadLength), 0);
        clientPacket.fileNameLength = ntohs(fileNameLength);

        clientPacket.fileName =(char*)calloc(clientPacket.fileNameLength,sizeof(char));
        recv(clientSocket, clientPacket.fileName, clientPacket.fileNameLength, 0);

        recv(clientSocket, &payloadLength, sizeof(payloadLength), 0);
        clientPacket.payloadLength = ntohs(payloadLength);

        clientPacket.payload =(char*)calloc(clientPacket.payloadLength,sizeof(char));
        recv(clientSocket, clientPacket.payload, payloadLength, 0);

        switch (clientPacket.type) {
            case HELLO:
                createClientDirectory(clientPacket.payload);
            break;
        }

        showPacketServer(clientPacket);
        free(clientPacket.fileName);
        free(clientPacket.payload);
    //}
}

void startServerSocket() {
    int serverSocket = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT_NUMBER);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(serverSocket,(struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket,MAX_SERVER_QUEUE_SIZE);

    cout << "Servidor inicializado com sucesso." << endl << "Escutando na porta " << SERVER_PORT_NUMBER << "..." << endl;
    receivePacket(serverSocket);

    close(serverSocket);
}

int main(int argc, char *argv[]) {
    createServerDirectory();
    startServerSocket();
    return 0;
}