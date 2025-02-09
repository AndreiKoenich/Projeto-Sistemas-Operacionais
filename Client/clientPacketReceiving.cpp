#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>

#include "clientConstants.hpp"
#include "clientUtils.hpp"
#include "packetEnum.hpp"

void receiveDownloadPacket(int clientSocket, string username) {
    DownloadPacket clientPacket;
    uint16_t payloadLength;
    uint16_t fileNameLength;

    clientPacket.packetType = DOWNLOAD;

    recv(clientSocket, &fileNameLength, sizeof(fileNameLength), 0);
    clientPacket.fileNameLength = ntohs(fileNameLength);

    clientPacket.fileName =(char*)calloc(clientPacket.fileNameLength,sizeof(char));
    recv(clientSocket, clientPacket.fileName, clientPacket.fileNameLength, 0);

    recv(clientSocket, &payloadLength, sizeof(payloadLength), 0);
    clientPacket.payloadLength = ntohs(payloadLength);

    clientPacket.payload =(char*)calloc(clientPacket.payloadLength,sizeof(char));
    recv(clientSocket, clientPacket.payload, payloadLength, 0);

    //showDownloadPacketClient(clientPacket);

    char filePath[FULL_DIRECTORY_NAME_SIZE];
    memset(filePath,0,sizeof(filePath));
    getcwd(filePath,FULL_DIRECTORY_NAME_SIZE);
    strcat(filePath,"/");
    strcat(filePath,clientPacket.fileName);

    /*
    strcat(filePath,CLIENT_DIRECTORY_PREFIX);

    char* usernameStr = (char*)calloc(username.length()+1,sizeof(char));
    username.copy(usernameStr,username.length());
    usernameStr[username.length()] = '\0';
    strcat(filePath,usernameStr);
    strcat(filePath,"/");
    strcat(filePath,clientPacket.fileName);
    */

    FILE *selectedFile;
    if((selectedFile = fopen(filePath, "wb")) == NULL) {
        cout << "Erro na criacao ou abertura do arquivo para escrita no diretorio do cliente, ao tentar executar o comando download." << endl;
        exit(1);
    }

    if(fwrite(clientPacket.payload, sizeof(char),clientPacket.payloadLength-1,selectedFile) != (size_t)clientPacket.payloadLength-1) {
        cout << "Erro na escrita do arquivo, ao utilizar o comando download." << endl;
        exit(1);
    }

    fclose(selectedFile);
    //free(usernameStr);
    free(clientPacket.fileName);
    free(clientPacket.payload);

    cout << "Arquivo recebido com sucesso do servidor e armazenado em:\n" << filePath << endl;
    cout << "Pressione qualquer tecla para continuar." << endl;
    getch_();
}

void receiveListServerPacket(int clientSocket) {

    ListServerPacket clientPacket;
    uint16_t payloadLength;

    clientPacket.packetType = LIST_SERVER;

    recv(clientSocket, &payloadLength, sizeof(payloadLength), 0);
    clientPacket.payloadLength = ntohs(payloadLength);

    clientPacket.payload =(char*)calloc(clientPacket.payloadLength,sizeof(char));
    recv(clientSocket, clientPacket.payload, payloadLength, 0);

    cout << clientPacket.payload << endl;

    //showListServerPacketClient(clientPacket);
    free(clientPacket.payload);

    cout << "Pressione qualquer tecla para continuar." << endl;
    getch_();
}

void receivePacketFromServer (int clientSocket, string username) {

    uint16_t packetType;

    recv(clientSocket, &packetType, sizeof(packetType), 0);
    packetType = ntohs(packetType);

    switch (packetType) {
        case DOWNLOAD:
            receiveDownloadPacket(clientSocket, username);
        break;
        case DOWNLOAD_ERROR:
            cout << "Erro ao tentar fazer download do arquivo no servidor." << endl;
            cout << "Pressione qualquer tecla para continuar." << endl;
            getch_();
        break;
        case LIST_SERVER:
            receiveListServerPacket(clientSocket);
        break;
    }
}