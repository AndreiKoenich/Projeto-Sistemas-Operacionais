#include <iostream>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <filesystem>
#include <sys/stat.h>
#include <iomanip>
#include <ctime>

#include "clientConstants.hpp"
#include "clientPacketSending.hpp"
#include "clientPacketReceiving.hpp"
#include "clientUtils.hpp"
#include "packetStruct.hpp"
#include "packetEnum.hpp"

void requestDownloadCommand(string username, string command, int clientSocket) {

    string fileName = command.substr(DOWNLOAD_COMMAND.length(), command.length() - DOWNLOAD_COMMAND.length());

    RequestDownloadPacket clientPacket;
    clientPacket.packetType = REQUEST_DOWNLOAD;
    clientPacket.fileNameLength = fileName.length()+1;
    clientPacket.fileName = (char*)calloc(clientPacket.fileNameLength,sizeof(char));
    fileName.copy(clientPacket.fileName,clientPacket.fileNameLength-1);
    clientPacket.fileName[clientPacket.fileNameLength-1] = '\0';

    sendRequestDownloadPacket(clientSocket, &clientPacket);
    cout << "Comando de requisicao de download executado com sucesso." << endl;
    receivePacketFromServer(clientSocket, username);

    free(clientPacket.fileName);
}

void uploadCommand(string command, int clientSocket) {

    string filePath = command.substr(UPLOAD_COMMAND.length(), command.length() - UPLOAD_COMMAND.length());

    UploadPacket clientPacket;    
    FILE *selectedFile;
    if((selectedFile = fopen(filePath.data(), "rb")) == NULL) {
        cout << "Erro na abertura do arquivo para envio ao servidor, ao tentar executar o comando upload." << endl;
        cout << "Pressione qualquer tecla para continuar." << endl;
        getch_();
        return;
    }

    reverse(filePath.begin(), filePath.end());

    clientPacket.fileNameLength = 1;
    for (int i = 0; filePath[i] != '/'; i++)
        clientPacket.fileNameLength++;

    clientPacket.fileName = (char*)calloc(clientPacket.fileNameLength,sizeof(char));
    filePath.copy(clientPacket.fileName, clientPacket.fileNameLength-1);
    reverseString(clientPacket.fileName);
    clientPacket.fileName[clientPacket.fileNameLength-1] = '\0';

    fseek(selectedFile, 0, SEEK_END);
	uint16_t fileLength=ftell(selectedFile);
	fseek(selectedFile, 0, SEEK_SET);

    clientPacket.packetType = UPLOAD;
    clientPacket.payloadLength = fileLength;
	clientPacket.payload =(char*)calloc(fileLength,sizeof(char));

    if(fread(clientPacket.payload, sizeof(char), fileLength, selectedFile) != fileLength) {
        cout << "Erro na leitura do arquivo para inserir os dados em buffer, ao tentar executar o comando upload." << endl;
        cout << "Pressione qualquer tecla para continuar." << endl;
        fclose(selectedFile);
        getch_();
        return;
    }

    clientPacket.payload[fileLength-1] = '\0';

    fclose(selectedFile);
    sendUploadPacket(clientSocket, &clientPacket);
    free(clientPacket.fileName);
    free(clientPacket.payload);
    cout << "Comando upload executado com sucesso. Pressione qualquer tecla para continuar." << endl;
    getch_();
}

void requestListServerCommand(string username, int clientSocket) {

    RequestListServerPacket clientPacket;
    clientPacket.packetType = REQUEST_LIST_SERVER;
    sendRequestListServerPacket(clientSocket, &clientPacket);
    receivePacketFromServer(clientSocket, username);
}

void listClientCommand(string username) {

    namespace fs = std::filesystem;
    string directoryPath(CLIENT_DIRECTORY_PREFIX);
    directoryPath += username;

    try {
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (fs::is_regular_file(entry.path())) {
                struct stat fileStat;
                if (stat(entry.path().c_str(), &fileStat) == 0) {
                    cout << "\nArquivo: " << entry.path().filename() << "\n";
                    cout << "  Modification Time (mtime): " << timeToString(fileStat.st_mtime) << "\n";
                    cout << "  Access Time (atime): " << timeToString(fileStat.st_atime) << "\n";
                    cout << "  Change Time (ctime): " << timeToString(fileStat.st_ctime) << "\n";
                    cout << "-----------------------------------------\n";
                } else {
                    cerr << "Erro ao obter estatisticas do arquivo: " << entry.path() << "\n";
                }
            }
        }
    } catch (const exception& e) {
        cerr << "Erro ao acessar o diretorio: " << e.what() << "\n";
    }

    cout << "\nPressione qualquer tecla para continuar." << endl;
    getch_();
}

void deleteCommand (string username, string command) {
    string directoryPath(CLIENT_DIRECTORY_PREFIX);
    directoryPath += username;
    string fileName = command.substr(DELETE_COMMAND.length(), command.length() - DELETE_COMMAND.length());
    directoryPath += ("/" + fileName);

    char* directoryPathStr = (char*)calloc(sizeof(char), directoryPath.length()+1);
    directoryPath.copy(directoryPathStr,directoryPath.length());
    directoryPathStr[directoryPath.length()] = '\0';

    if (remove(directoryPathStr) != 0)
        cerr << "Erro ao tentar remover o arquivo no diretorio " << directoryPathStr << endl;

    else
        cout << "\nArquivo " << directoryPath << " removido com sucesso." << endl;

    free(directoryPathStr);

    cout << "\nPressione qualquer tecla para continuar." << endl;
    getch_();
}

void exitCommand(int clientSocket) {
    ByePacket clientPacket;  
    clientPacket.packetType = BYE;
    sendByePacket(clientSocket, &clientPacket);
    close(clientSocket);
}