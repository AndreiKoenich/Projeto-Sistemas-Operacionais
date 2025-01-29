#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <filesystem>
#include <sstream>  
#include <iomanip>
#include <ctime>

#include "packetStruct.hpp"
#include "packetEnum.hpp"
#include "serverConstants.hpp"
#include "serverUtils.hpp"
#include "serverPacketSending.hpp"

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

void receiveRequestDownloadPacket(int clientSocket, string username) {
    DownloadPacket clientPacket;
    uint16_t fileNameLength;

    clientPacket.packetType = DOWNLOAD;

    recv(clientSocket, &fileNameLength, sizeof(fileNameLength), 0);
    clientPacket.fileNameLength = ntohs(fileNameLength);

    clientPacket.fileName =(char*)calloc(clientPacket.fileNameLength,sizeof(char));
    recv(clientSocket, clientPacket.fileName, clientPacket.fileNameLength, 0);

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
    if((selectedFile = fopen(filePath, "rb")) == NULL) {
        DownloadErrorPacket errorPacket;
        errorPacket.packetType = DOWNLOAD_ERROR;
        sendDownloadErrorPacket(clientSocket, &errorPacket);

        free(usernameStr);
        free(clientPacket.fileName);
        return;
    }

    fseek(selectedFile, 0, SEEK_END);
	uint16_t fileLength=ftell(selectedFile);
	fseek(selectedFile, 0, SEEK_SET);

    clientPacket.payloadLength = fileLength;
	clientPacket.payload =(char*)calloc(fileLength,sizeof(char));

    if(fread(clientPacket.payload, sizeof(char),clientPacket.payloadLength-1,selectedFile) != (size_t)clientPacket.payloadLength-1) 
        cout << "Erro na leitura do arquivo nao-sincronizado, ao utilizar o comando download, para armazenar os dados em buffer." << endl;
    
    else
        sendDownloadPacket(clientSocket, &clientPacket);

    fclose(selectedFile);
    free(usernameStr);
    free(clientPacket.fileName);
    free(clientPacket.payload);
}

void receiveRequestListServerPacket(int clientSocket, string username) {
    ListServerPacket clientPacket;
    clientPacket.packetType = LIST_SERVER;

    char* usernameStr = (char*)calloc(username.length()+1,sizeof(char));
    username.copy(usernameStr,username.length());
    usernameStr[username.length()] = '\0';

    char clientDirectoryName[FULL_DIRECTORY_NAME_SIZE];
    memset(clientDirectoryName,0,sizeof(clientDirectoryName));
    getcwd(clientDirectoryName,FULL_DIRECTORY_NAME_SIZE);
    strcat(clientDirectoryName,CLIENT_DIRECTORY_NAME);
    strcat(clientDirectoryName,usernameStr);

    namespace fs = std::filesystem;
    stringstream payload;

    try {
        for (const auto& entry : fs::directory_iterator(clientDirectoryName)) {
            if (fs::is_regular_file(entry.path())) {
                struct stat fileStat;
                if (stat(entry.path().c_str(), &fileStat) == 0) {
                    payload << "\nArquivo: " << entry.path().filename() << "\n";
                    payload << "  Modification Time (mtime): " << timeToString(fileStat.st_mtime) << "\n";
                    payload << "  Access Time (atime): " << timeToString(fileStat.st_atime) << "\n";
                    payload << "  Change Time (ctime): " << timeToString(fileStat.st_ctime) << "\n";
                    payload << "-----------------------------------------\n";
                } else {
                    payload << "Erro ao obter estatisticas do arquivo: " << entry.path() << "\n";
                }
            }
        }
    } catch (const exception& e) {
        cerr << "Erro ao acessar o diretorio: " << e.what() << "\n";
    }

    clientPacket.payloadLength = payload.str().length()+1;
    clientPacket.payload = (char*)calloc(sizeof(char), clientPacket.payloadLength);
    payload.str().copy(clientPacket.payload,clientPacket.payloadLength-1);
    clientPacket.payload[clientPacket.payloadLength-1] = '\0';

    sendListServerPacket(clientSocket, &clientPacket);

    free(clientPacket.payload);
    free(usernameStr);
}

void receiveUploadPacket (int clientSocket, string username) {
    UploadPacket clientPacket;
    uint16_t payloadLength;
    uint16_t fileNameLength;

    clientPacket.packetType = UPLOAD;

    recv(clientSocket, &fileNameLength, sizeof(fileNameLength), 0);
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