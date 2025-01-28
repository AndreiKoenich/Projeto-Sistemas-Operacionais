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

void showByePacketServer () {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote bye recebido com sucesso.\n" << endl;
    cout << "Tipo:" << BYE << endl;
    cout << "-----------------------------------------------\n" << endl;
}