#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>
#include <sys/stat.h>

#include "clientConstants.hpp"
#include "clientUtils.hpp"
#include "packetEnum.hpp"
#include "clientStruct.hpp"
#include "propagationInfo.hpp"

extern stringstream textToImpress;

extern pthread_mutex_t mutexClientDirectory;
extern pthread_mutex_t mutexClientSocket;

propagationInfo lastUploadPropagation;
propagationInfo lastDeletePropagation;

void receiveDownloadPacket(int clientSocket, string username) {

    DownloadPacket clientPacket;
    uint16_t payloadLength;
    uint16_t fileNameLength;

    clientPacket.packetType = DOWNLOAD;

    pthread_mutex_lock(&mutexClientSocket);

    recv(clientSocket, &fileNameLength, sizeof(fileNameLength), 0);
    clientPacket.fileNameLength = ntohs(fileNameLength);

    clientPacket.fileName =(char*)calloc(clientPacket.fileNameLength,sizeof(char));
    recv(clientSocket, clientPacket.fileName, clientPacket.fileNameLength, 0);

    recv(clientSocket, &payloadLength, sizeof(payloadLength), 0);
    clientPacket.payloadLength = ntohs(payloadLength);

    clientPacket.payload =(char*)calloc(clientPacket.payloadLength,sizeof(char));
    recv(clientSocket, clientPacket.payload, payloadLength, 0);

    pthread_mutex_unlock(&mutexClientSocket);

    //showDownloadPacketClient(clientPacket);

    char filePath[FULL_DIRECTORY_NAME_SIZE];
    memset(filePath,0,sizeof(filePath));
    getcwd(filePath,FULL_DIRECTORY_NAME_SIZE);
    strcat(filePath,"/");
    strcat(filePath,clientPacket.fileName);

    FILE *selectedFile;
    if((selectedFile = fopen(filePath, "wb")) == NULL) {
        textToImpress << "\n\nErro na criacao ou abertura do arquivo para escrita no diretorio do cliente, ao tentar executar o comando download." << endl;
        //exit(1);
    }

    if(fwrite(clientPacket.payload, sizeof(char),clientPacket.payloadLength-1,selectedFile) != (size_t)clientPacket.payloadLength-1) {
        textToImpress << "\n\nErro na escrita do arquivo, ao utilizar o comando download." << endl;
        //exit(1);
    }

    fclose(selectedFile);
    free(clientPacket.fileName);
    free(clientPacket.payload);

    textToImpress << "Arquivo recebido com sucesso do servidor e armazenado em\n" << filePath << endl;
}

void receiveUploadPropagationPacket(int clientSocket, string username) {

    pthread_mutex_lock(&mutexClientDirectory);

    UploadPacket clientPacket;
    uint16_t payloadLength;
    uint16_t fileNameLength;

    clientPacket.packetType = UPLOAD_PROPAGATION;

    pthread_mutex_lock(&mutexClientSocket);

    recv(clientSocket, &fileNameLength, sizeof(fileNameLength), 0);
    clientPacket.fileNameLength = ntohs(fileNameLength);

    clientPacket.fileName =(char*)calloc(clientPacket.fileNameLength,sizeof(char));
    recv(clientSocket, clientPacket.fileName, clientPacket.fileNameLength, 0);

    recv(clientSocket, &payloadLength, sizeof(payloadLength), 0);
    clientPacket.payloadLength = ntohs(payloadLength);

    if (clientPacket.payloadLength != 0) {
        clientPacket.payload =(char*)calloc(clientPacket.payloadLength,sizeof(char));
        recv(clientSocket, clientPacket.payload, payloadLength, 0);
    }
    //clientPacket.payload[clientPacket.payloadLength-1] = '\0';

    pthread_mutex_unlock(&mutexClientSocket);

    //showReceivedPropagationPacketClient(clientPacket);

    char filePath[FULL_DIRECTORY_NAME_SIZE];
    memset(filePath,0,sizeof(filePath));
    getcwd(filePath,FULL_DIRECTORY_NAME_SIZE);
    strcat(filePath,"/");
    strcat(filePath,CLIENT_DIRECTORY_PREFIX);
    char* usernameStr = (char*)calloc(username.length()+1,sizeof(char));
    username.copy(usernameStr,username.length());
    usernameStr[username.length()] = '\0';
    strcat(filePath,usernameStr);
    strcat(filePath,"/");
    strcat(filePath,clientPacket.fileName);

    //cout << "Diretorio de recebimento de propagacao:\n" << filePath << endl;

    FILE *selectedFile;
    if((selectedFile = fopen(filePath, "wb")) == NULL) {
        textToImpress << "\n\nErro na criacao ou abertura do arquivo para escrita no diretorio do cliente na propagacao de upload." << endl;
        //exit(1);
    }
 
    if(clientPacket.payloadLength != 0 && fwrite(clientPacket.payload, sizeof(char),clientPacket.payloadLength,selectedFile) != (size_t)clientPacket.payloadLength) {
        textToImpress << "\n\nErro na escrita do arquivo ao realizar a propagacao do upload." << endl;
        //exit(1);
    }

    lastUploadPropagation.filename = clientPacket.fileName;

    if (clientPacket.payloadLength != 0)
        lastUploadPropagation.payload = clientPacket.payload;
    else
        lastUploadPropagation.payload = "";

    fclose(selectedFile);
    free(usernameStr);
    free(clientPacket.fileName);

    if (clientPacket.payloadLength != 0)
        free(clientPacket.payload);

    pthread_mutex_unlock(&mutexClientDirectory);
}

void receiveDeletePropagationPacket(int clientSocket, string username) {

    pthread_mutex_lock(&mutexClientDirectory);

    RequestDeletePacket clientPacket;
    uint16_t fileNameLength;

    clientPacket.packetType = DELETE_PROPAGATION;

    pthread_mutex_lock(&mutexClientSocket);

    recv(clientSocket, &fileNameLength, sizeof(fileNameLength), 0);
    clientPacket.fileNameLength = ntohs(fileNameLength);

    clientPacket.fileName =(char*)calloc(clientPacket.fileNameLength,sizeof(char));

    pthread_mutex_unlock(&mutexClientSocket);

    recv(clientSocket, clientPacket.fileName, clientPacket.fileNameLength, 0);

    char filePath[FULL_DIRECTORY_NAME_SIZE];
    memset(filePath,0,sizeof(filePath));
    getcwd(filePath,FULL_DIRECTORY_NAME_SIZE);
    strcat(filePath,"//");
    strcat(filePath,CLIENT_DIRECTORY_PREFIX);
    char* usernameStr = (char*)calloc(username.length()+1,sizeof(char));
    username.copy(usernameStr,username.length());
    usernameStr[username.length()] = '\0';
    strcat(filePath,usernameStr);
    strcat(filePath,"//");
    strcat(filePath,clientPacket.fileName);

    if (remove(filePath) != 0)
        textToImpress << "\n\nErro ao tentar remover o arquivo no diretorio " << filePath << " por notificacao de propagacao." << endl;
    else
        //cout << "Arquivo " << filePath << " removido com sucesso." << endl;

    lastDeletePropagation.filename = clientPacket.fileName;
    lastDeletePropagation.payload = "";

    free(usernameStr);
    free(clientPacket.fileName);

    pthread_mutex_unlock(&mutexClientDirectory);
}

void receiveListServerPacket(int clientSocket) {

    ListServerPacket clientPacket;
    uint16_t payloadLength;

    clientPacket.packetType = LIST_SERVER;

    recv(clientSocket, &payloadLength, sizeof(payloadLength), 0);
    clientPacket.payloadLength = ntohs(payloadLength);

    clientPacket.payload =(char*)calloc(clientPacket.payloadLength,sizeof(char));
    recv(clientSocket, clientPacket.payload, payloadLength, 0);

    textToImpress << clientPacket.payload << endl;

    //showListServerPacketClient(clientPacket);
    free(clientPacket.payload); 
}

void* receivePacketFromServerLoop (void* parameters) {

    clientStruct *menuParameters = (clientStruct*) parameters;

    uint16_t packetType;

    while (true) {

        recv(menuParameters->clientSocket, &packetType, sizeof(packetType), 0);
        packetType = ntohs(packetType);

        switch (packetType) {
        
            case DOWNLOAD:
                receiveDownloadPacket(menuParameters->clientSocket, menuParameters->username);
            break;
            case DOWNLOAD_ERROR:
                textToImpress << "\n\nErro ao tentar fazer download do arquivo no servidor." << endl;
            break;
            case LIST_SERVER:
                receiveListServerPacket(menuParameters->clientSocket);
            break;
            case UPLOAD_PROPAGATION:
                receiveUploadPropagationPacket(menuParameters->clientSocket, menuParameters->username);
            break;
            case DELETE_PROPAGATION:
                receiveDeletePropagationPacket(menuParameters->clientSocket, menuParameters->username);
            break;
        }
    }

    return NULL;
}

