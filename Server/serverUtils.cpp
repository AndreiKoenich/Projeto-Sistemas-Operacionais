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

string timeToString(time_t rawTime) {
    struct tm timeInfo;
    localtime_r(&rawTime, &timeInfo);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeInfo);
    return string(buffer);
}

void createServerDirectory() {
    struct stat st = {0};
    if (stat(SERVER_DIRECTORY_NAME, &st) == -1) 
        if (mkdir(SERVER_DIRECTORY_NAME, 0777) == -1) {
            cerr << "Erro ao criar o diretorio " << SERVER_DIRECTORY_NAME << "para sincronizacao dos arquivos." << endl;
            exit(1);
        }            
}

string showPacketType(int value) {
    switch (value) {
        case HELLO:
            return "HELLO";
        case UPLOAD:
            return "UPLOAD";
        case UPLOAD_INOTIFY:
            return "UPLOAD_INOTIFY";
        case UPLOAD_PROPAGATION:
            return "UPLOAD_PROPAGATION";
        case REQUEST_DOWNLOAD:
            return "REQUEST_DOWNLOAD";
        case DOWNLOAD:
            return "DOWNLOAD";
        case DOWNLOAD_ERROR:
            return "DOWNLOAD_ERROR";
        case DELETE:
            return "DELETE";
        case DELETE_INOTIFY:
            return "DELETE_INOTIFY";
        case DELETE_PROPAGATION:
            return "DELETE_PROPAGATION";
        case REQUEST_LIST_SERVER:
            return "REQUEST_LIST_SERVER";
        case LIST_SERVER:
            return "LIST_SERVER";
        case BYE:
            return "BYE";
        default:
            return "UNKNOWN";
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
            cerr << "Erro ao criar o diretorio remoto do cliente." << endl;
            exit(1);
        }            
}

void showHelloPacketServer (HelloPacket clientPacket) {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote HELLO recebido com sucesso.\n" << endl;
    cout << "Tipo:" << showPacketType(clientPacket.packetType) << endl;
    cout << "Tamanho do nome do usuario:" << clientPacket.usernameLength << endl;
    cout << "Conteudo:" << clientPacket.username << endl;
    cout << "-----------------------------------------------\n" << endl;
}

void showUploadPacketServer (UploadPacket clientPacket) {
    cout << "\n-----------------------------------------------" << endl;

    if (clientPacket.packetType == UPLOAD)
        cout << "Pacote UPLOAD recebido com sucesso.\n" << endl;
    else if (clientPacket.packetType == UPLOAD_INOTIFY)
        cout << "Pacote UPLOAD_INOTIFY recebido com sucesso.\n" << endl;
    else if (clientPacket.packetType == UPLOAD_PROPAGATION)
        cout << "Pacote UPLOAD_PROPAGATION enviado com sucesso.\n" << endl;
    cout << "Tipo:" << showPacketType(clientPacket.packetType) << endl;
    cout << "Tamanho do nome do arquivo:" << clientPacket.fileNameLength << endl;
    cout << "Nome do arquivo:" << clientPacket.fileName << endl;
    cout << "Tamanho do payload:" << clientPacket.payloadLength << endl; 

    if (clientPacket.payloadLength != 0)      
        cout << "Conteudo:" << clientPacket.payload << endl;
    else
        cout << "O conteudo do pacote esta vazio." << endl;
        
    cout << "-----------------------------------------------\n" << endl;
}

void showDownloadPacketServer (DownloadPacket clientPacket) {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote DOWNLOAD enviado com sucesso.\n" << endl;
    cout << "Tipo:" << showPacketType(clientPacket.packetType) << endl;
    cout << "Tamanho do nome do arquivo:" << clientPacket.fileNameLength << endl;
    cout << "Nome do arquivo:" << clientPacket.fileName << endl;
    cout << "Tamanho do payload:" << clientPacket.payloadLength << endl;       
    cout << "Conteudo:" << clientPacket.payload << endl;
    cout << "-----------------------------------------------\n" << endl;
}

void showByePacketServer (string username) {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote BYE do usuario "<< username <<" recebido com sucesso.\n" << endl;
    cout << "Tipo: BYE" << endl;
    cout << "-----------------------------------------------\n" << endl;
}

void showDownloadErrorPacketServer() {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote de erro de DOWNLOAD enviado com sucesso.\n" << endl;
    cout << "Tipo:" << DOWNLOAD_ERROR << endl;
    cout << "-----------------------------------------------\n" << endl;
}

void showListServerPacket(ListServerPacket clientPacket) {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote LIST SERVER enviado com sucesso.\n" << endl;
    cout << "Tipo:" << showPacketType(clientPacket.packetType) << endl;
    cout << "Tamanho do payload:" << clientPacket.payloadLength << endl;       
    cout << "Conteudo:" << clientPacket.payload << endl;
    cout << "-----------------------------------------------\n" << endl;   
}

void showRequestDeletePacketServer(RequestDeletePacket clientPacket) {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote de requisicao de DELETE enviado com sucesso.\n" << endl;
    cout << "Tipo:" << showPacketType(clientPacket.packetType) << endl;
    cout << "Tamanho do nome do arquivo:" << clientPacket.fileNameLength << endl;
    cout << "Nome do arquivo:" << clientPacket.fileName << endl;
    cout << "-----------------------------------------------\n" << endl;   
}

void showRequestDownloadPacketServer (DownloadPacket clientPacket) {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote de requisicao de DOWNLOAD recebido com sucesso.\n" << endl;
    cout << "Tipo:" << showPacketType(clientPacket.packetType) << endl;    
    cout << "Tamanho do nome do arquivo:" << clientPacket.fileNameLength << endl;
    cout << "Nome do arquivo:" << clientPacket.fileName << endl;
    cout << "-----------------------------------------------\n" << endl;  
}


