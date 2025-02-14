#include <iostream>
#include <string>
#include <unistd.h>
#include <termios.h> 
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sstream>

#include "clientConstants.hpp"
#include "clientCommands.hpp"
#include "clientPacketSending.hpp"
#include "packetEnum.hpp"
#include "packetStruct.hpp"

extern stringstream textToImpress;

using namespace std;

void reverseString(char* str)
{
    int strSize = strlen(str);
    char aux[strSize+1];
    strcpy(aux,str);
    for (int i = 0; str[i] != '\0'; i++)
        str[i] = aux[strSize-i-1];
}

string timeToString(time_t rawTime) {
    struct tm timeInfo;
    localtime_r(&rawTime, &timeInfo);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeInfo);
    return string(buffer);
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

void showRequestDownloadPacketClient(RequestDownloadPacket clientPacket) {

    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote de requisicao de DOWNLOAD enviado com sucesso.\n" << endl;
    cout << "Tipo:" << showPacketType(clientPacket.packetType) << endl;    
    cout << "Tamanho do nome do arquivo:" << clientPacket.fileNameLength << endl;
    cout << "Nome do arquivo:" << clientPacket.fileName << endl;
    cout << "-----------------------------------------------\n" << endl;   
}

void showDownloadPacketClient (DownloadPacket clientPacket) {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote DOWNLOAD recebido com sucesso.\n" << endl;
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

void showUploadPacketClient (UploadPacket clientPacket) {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote UPLOAD enviado com sucesso.\n" << endl;
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

void showHelloPacketClient (HelloPacket clientPacket) {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote HELLO enviado com sucesso.\n" << endl;
    cout << "Tipo:" << showPacketType(clientPacket.packetType) << endl;
    cout << "Tamanho do nome do usuario:" << clientPacket.usernameLength << endl;
    cout << "Conteudo:" << clientPacket.username << endl;
    cout << "-----------------------------------------------\n" << endl;
}

void showByePacketClient (ByePacket clientPacket) {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote BYE enviado com sucesso.\n" << endl;
    cout << "Tipo:" << showPacketType(clientPacket.packetType) << endl;
    cout << "-----------------------------------------------\n" << endl;
}

void showRequestListServerPacketClient(RequestListServerPacket clientPacket) {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote de REQUEST LIST SERVER enviado com sucesso.\n" << endl;
    cout << "Tipo:" << showPacketType(clientPacket.packetType) << endl;
    cout << "-----------------------------------------------\n" << endl;    
}

void showListServerPacketClient(ListServerPacket clientPacket) {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote LIST SERVER recebido com sucesso.\n" << endl;
    cout << "Tipo:" << showPacketType(clientPacket.packetType) << endl;
    cout << "Tamanho do payload:" << clientPacket.payloadLength << endl;       
    cout << "Conteudo:" << clientPacket.payload << endl;
    cout << "-----------------------------------------------\n" << endl;    
}

void showRequestDeletePacketClient(RequestDeletePacket clientPacket) {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote de requisicao de DELETE enviado com sucesso.\n" << endl;
    cout << "Tipo:" << showPacketType(clientPacket.packetType) << endl;
    cout << "Tamanho do nome do arquivo:" << clientPacket.fileNameLength << endl;
    cout << "Nome do arquivo:" << clientPacket.fileName << endl;
    cout << "-----------------------------------------------\n" << endl;   
}

void showReceivedPropagationPacketClient (UploadPacket clientPacket) {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote UPLOAD_PROPAGATION recebido com sucesso.\n" << endl;
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

void createRemoteDirectory (char username[], int clientSocket) {
    HelloPacket clientPacket;
    clientPacket.packetType = HELLO;
    clientPacket.usernameLength = strlen(username)+1;
    clientPacket.username = (char*)calloc(clientPacket.usernameLength,sizeof(char));

    strcpy(clientPacket.username, username);
    clientPacket.username[clientPacket.usernameLength-1] = '\0';

    sendHelloPacket(clientSocket, &clientPacket);
    free(clientPacket.username);
}

void createClientDirectory(char username[]) {

    struct stat st = {0};

    int directory_name_length = strlen(username)+strlen(CLIENT_DIRECTORY_PREFIX)+1;
    char client_directory_name[directory_name_length];
    memset(client_directory_name,0,sizeof(client_directory_name));
    strcat(client_directory_name,CLIENT_DIRECTORY_PREFIX);
    strcat(client_directory_name,username);

    if (stat(client_directory_name, &st) == -1) 
        if (mkdir(client_directory_name, 0777) == -1) {
            textToImpress << "Erro ao criar o diretorio " << client_directory_name << "para sincronizacao dos arquivos." << endl;
            exit(1);
        }
}

void helpMenu() {
    textToImpress << "LISTA DE COMANDOS:" << endl;
    textToImpress << "-----------------------------------------------\n" << endl;

    textToImpress << "COMANDO:" << endl;
    textToImpress << "upload <path/filename.ext>" << endl;
    textToImpress << "DESCRICAO:" << endl;
    textToImpress << "Envia o arquivo filename.ext para o servidor, colocando-o no “sync_dir” do  servidor e propagando-o para todos os dispositivos daquele usuario.e.g. upload /home/user/MyFolder/filename.ext\n"  << endl;

    textToImpress << "COMANDO:" << endl;
    textToImpress << "download <filename.ext>" << endl;
    textToImpress << "DESCRICAO:" << endl;
    textToImpress << "Faz uma copia nao sincronizada do arquivo filename.ext do servidor para o diretorio local (de onde o servidor foi chamado). e.g. download mySpreadsheet.xlsx\n" << endl;

    textToImpress << "COMANDO:" << endl;
    textToImpress << "delete <filename.ext>" << endl;
    textToImpress << "DESCRICAO:" << endl;
    textToImpress << "Exclui o arquivo <filename.ext> de “sync_dir”.\n"  << endl;

    textToImpress << "COMANDO:" << endl;
    textToImpress << "list_server" << endl;
    textToImpress << "DESCRICAO:" << endl;
    textToImpress << "Lista os arquivos salvos no servidor, associados ao usuario.\n"  << endl;

    textToImpress << "COMANDO:" << endl;
    textToImpress << "list_client" << endl;
    textToImpress << "DESCRICAO:" << endl;
    textToImpress << "Lista os arquivos salvos no diretorio “sync_dir”.\n"  << endl;

    textToImpress << "COMANDO:" << endl;
    textToImpress << "exit" << endl;
    textToImpress << "DESCRICAO:" << endl;
    textToImpress << "Fecha a sessao com o servidor.\n"  << endl;
}