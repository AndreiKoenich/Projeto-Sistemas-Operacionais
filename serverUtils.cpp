#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "packetStruct.hpp"
#include "packetEnum.hpp"
#include "serverConstants.hpp"
using namespace std;

void createServerDirectory() {
    struct stat st = {0};
    if (stat(SERVER_DIRECTORY_NAME, &st) == -1) 
        if (mkdir(SERVER_DIRECTORY_NAME, 0777) == -1) {
            cout << "Erro ao criar o diretorio " << SERVER_DIRECTORY_NAME << "para sincronizacao dos arquivos." << endl;
            exit(1);
        }            
}

void createClientDirectory(char username[]) {

    char clientDirectoryName[FULL_DIRECTORY_NAME_SIZE];
    memset(clientDirectoryName,0,sizeof(clientDirectoryName));

    getcwd(clientDirectoryName,FULL_DIRECTORY_NAME_SIZE);
    strcat(clientDirectoryName,CLIENT_DIRECTORY_NAME);
    strcat(clientDirectoryName,username);

    struct stat st = {0};
    if (stat(clientDirectoryName, &st) == -1) 
        if (mkdir(clientDirectoryName, 0777) == -1) {
            cout << "Erro ao criar o diretorio remoto do cliente." << endl;
            exit(1);
        }            
}

void showHelloPacketServer (HelloPacket clientPacket) {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote hello recebido com sucesso.\n" << endl;
    cout << "Tipo:" << clientPacket.packetType << endl;
    cout << "Tamanho do nome do usuario:" << clientPacket.usernameLength << endl;
    cout << "Conteudo:" << clientPacket.username << endl;
    cout << "-----------------------------------------------\n" << endl;
}

void receiveHelloPacket (int clientSocket) {
    HelloPacket clientPacket;
    uint16_t usernameLength;

    clientPacket.packetType = HELLO;

    recv(clientSocket, &usernameLength, sizeof(usernameLength), 0);
    clientPacket.usernameLength = ntohs(usernameLength);

    clientPacket.username =(char*)calloc(clientPacket.usernameLength,sizeof(char));
    recv(clientSocket, clientPacket.username, usernameLength, 0);

    showHelloPacketServer(clientPacket);

    char clientDirectoryName[FULL_DIRECTORY_NAME_SIZE];
    memset(clientDirectoryName,0,sizeof(clientDirectoryName));

    getcwd(clientDirectoryName,FULL_DIRECTORY_NAME_SIZE);
    strcat(clientDirectoryName,CLIENT_DIRECTORY_NAME);
    strcat(clientDirectoryName,clientPacket.username);

    struct stat st = {0};
    if (stat(clientDirectoryName, &st) == -1) 
        if (mkdir(clientDirectoryName, 0777) == -1) {
            cout << "Erro ao criar o diretorio remoto do cliente." << endl;
            exit(1);
        }  

    free(clientPacket.username);
}

void showUploadPacketServer (UploadPacket clientPacket) {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote upload recebido com sucesso.\n" << endl;
    cout << "Tipo:" << clientPacket.packetType << endl;
    cout << "Tamanho do nome do arquivo:" << clientPacket.fileNameLength << endl;
    cout << "Nome do arquivo:" << clientPacket.fileName << endl;
    cout << "Tamanho do payload:" << clientPacket.payloadLength << endl;       
    cout << "Conteudo:" << clientPacket.payload << endl;
    cout << "-----------------------------------------------\n" << endl;
}

void receiveUploadPacket (int clientSocket) {
    UploadPacket clientPacket;
    uint16_t payloadLength;
    uint16_t fileNameLength;

    clientPacket.packetType = UPLOAD;

    recv(clientSocket, &fileNameLength, sizeof(payloadLength), 0);
    clientPacket.fileNameLength = ntohs(fileNameLength);

    clientPacket.fileName =(char*)calloc(clientPacket.fileNameLength,sizeof(char));
    recv(clientSocket, clientPacket.fileName, clientPacket.fileNameLength, 0);

    recv(clientSocket, &payloadLength, sizeof(payloadLength), 0);
    clientPacket.payloadLength = ntohs(payloadLength);

    clientPacket.payload =(char*)calloc(clientPacket.payloadLength,sizeof(char));
    recv(clientSocket, clientPacket.payload, payloadLength, 0);

    showUploadPacketServer(clientPacket);

    free(clientPacket.fileName);
    free(clientPacket.payload);
}