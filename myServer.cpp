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
#include "serverPacketReceiving.hpp"

using namespace std;

void receivePacket (int serverSocket) {

    int clientSocket = accept(serverSocket, nullptr, nullptr);
    cout << "Nova conexao aceita com sucesso." << endl;

    string username("");

    uint16_t packetType;

    while (true) {
        recv(clientSocket, &packetType, sizeof(packetType), 0);
        packetType = ntohs(packetType);

        switch (packetType) {
            case HELLO:
                username = receiveHelloPacket(clientSocket);
            break;
            case UPLOAD:
                receiveUploadPacket(clientSocket, username);
            break;
        }
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

    cout << "Servidor inicializado com sucesso." << endl << "Escutando na porta " << SERVER_PORT_NUMBER << "..." << endl;
    receivePacket(serverSocket);

    close(serverSocket);
}

int main(int argc, char *argv[]) {
    createServerDirectory();
    startServerSocket();
    return 0;
}