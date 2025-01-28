#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "packetStruct.hpp"
#include "packetEnum.hpp"
#include "serverConstants.hpp"
#include "serverUtils.hpp"

string receiveHelloPacket (int clientSocket) {
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

    string username(clientPacket.username);
    free(clientPacket.username);
    return username;
}

void receiveUploadPacket (int clientSocket, string username) {
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

    char filePath[FULL_DIRECTORY_NAME_SIZE];
    memset(filePath,0,sizeof(filePath));
    getcwd(filePath,FULL_DIRECTORY_NAME_SIZE);
    strcat(filePath,CLIENT_DIRECTORY_NAME);

    char* usernameStr = (char*)calloc(username.length()+1,sizeof(char));
    username.copy(usernameStr,username.length());
    usernameStr[username.length()] = '\0';
    strcat(filePath,usernameStr);
    strcat(filePath,"//");
    strcat(filePath,clientPacket.fileName);

    FILE *selectedFile;
    if((selectedFile = fopen(filePath, "wb")) == NULL) {
        cout << "Erro na criacao ou abertura do arquivo para escrita no diretorio do cliente, ao tentar executar o comando upload." << endl;
        exit(1);
    }

    if(fwrite(clientPacket.payload, sizeof(char),clientPacket.payloadLength-1,selectedFile) != (size_t)clientPacket.payloadLength-1) {
        cout << "Erro na escrita do arquivo, ao utilizar o comando upload." << endl;
        exit(1);
    }

    fclose(selectedFile);
    free(usernameStr);
    free(clientPacket.fileName);
    free(clientPacket.payload);
}