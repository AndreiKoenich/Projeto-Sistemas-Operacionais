#include <sys/socket.h>
#include <bits/stdc++.h>
#include <sys/stat.h>

#include "clientConstants.hpp"
#include "clientPacketSending.hpp"
#include "clientPacketReceiving.hpp"
#include "clientUtils.hpp"
#include "packetEnum.hpp"
#include "clientStruct.hpp"

extern pthread_mutex_t mutexClientDirectory;

void requestDownloadCommand(string command, clientStruct *menuParameters) {

    string username(menuParameters->username);
    string fileName = command.substr(DOWNLOAD_COMMAND.length(), command.length() - DOWNLOAD_COMMAND.length());

    RequestDownloadPacket clientPacket;
    clientPacket.packetType = REQUEST_DOWNLOAD;
    clientPacket.fileNameLength = fileName.length()+1;
    clientPacket.fileName = (char*)calloc(clientPacket.fileNameLength,sizeof(char));
    fileName.copy(clientPacket.fileName,clientPacket.fileNameLength-1);
    clientPacket.fileName[clientPacket.fileNameLength-1] = '\0';

    sendRequestDownloadPacket(menuParameters->clientSocket, &clientPacket);
    //receivePacketFromServer(menuParameters->clientSocket, username);

    free(clientPacket.fileName);
}

void uploadCommand(string command, clientStruct *menuParameters) {

    string filePath = command.substr(UPLOAD_COMMAND.length(), command.length() - UPLOAD_COMMAND.length());

    UploadPacket clientPacket;    
    FILE *selectedFile;
    if((selectedFile = fopen(filePath.data(), "rb")) == NULL) {
        cerr << "Erro na abertura do arquivo para envio ao servidor, ao tentar executar o comando upload." << endl;
                
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
        cerr << "Erro na leitura do arquivo para inserir os dados em buffer, ao tentar executar o comando upload." << endl;
        fclose(selectedFile);
        return;
    }

    if (fileLength != 0)
        clientPacket.payload[fileLength-1] = '\0';

    fclose(selectedFile);
    sendUploadPacket(menuParameters->clientSocket, &clientPacket);
    free(clientPacket.fileName);
    free(clientPacket.payload);

    //cout << "Comando upload executado com sucesso. Pressione qualquer tecla para continuar." << endl;
    
}

void requestListServerCommand(clientStruct *menuParameters) {

    string username(menuParameters->username);
    RequestListServerPacket clientPacket;
    clientPacket.packetType = REQUEST_LIST_SERVER;
    sendRequestListServerPacket(menuParameters->clientSocket, &clientPacket);
    //receivePacketFromServer(menuParameters->clientSocket, username);
}

void listClientCommand(clientStruct *menuParameters) {

    pthread_mutex_lock(&mutexClientDirectory);

    string username(menuParameters->username);
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

    pthread_mutex_unlock(&mutexClientDirectory);

    //cout << "\nPressione qualquer tecla para continuar." << endl;
    
}

void deleteCommand (string command, clientStruct *menuParameters) {

    string username(menuParameters->username);
    string directoryPath(CLIENT_DIRECTORY_PREFIX);
    directoryPath += username;
    string fileName = command.substr(DELETE_COMMAND.length(), command.length() - DELETE_COMMAND.length());
    directoryPath += ("/" + fileName);

    char* directoryPathStr = (char*)calloc(sizeof(char), directoryPath.length()+1);
    directoryPath.copy(directoryPathStr,directoryPath.length());
    directoryPathStr[directoryPath.length()] = '\0';

    if (remove(directoryPathStr) != 0)
        cerr << "Erro ao tentar remover o arquivo no diretorio " << directoryPathStr << endl;
    //else
        //cout << "\nArquivo " << directoryPath << " removido com sucesso." << endl;

    free(directoryPathStr);

    //cout << "\nPressione qualquer tecla para continuar." << endl;
    
}

void deleteInotify(string username, string fileName, int clientSocket) {

    RequestDeletePacket clientPacket;
    clientPacket.packetType = DELETE_INOTIFY;
    clientPacket.fileNameLength = fileName.length()+1;
    clientPacket.fileName = (char*)calloc(clientPacket.fileNameLength,sizeof(char));
    fileName.copy(clientPacket.fileName,clientPacket.fileNameLength-1);
    clientPacket.fileName[clientPacket.fileNameLength-1] = '\0';

    sendRequestDeletePacket(clientSocket, &clientPacket);
    //cout << "Comando de requisicao de delete executado com sucesso." << endl;

    free(clientPacket.fileName);
}