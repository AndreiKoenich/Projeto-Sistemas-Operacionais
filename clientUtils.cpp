#include <iostream>
#include <string>
#include <unistd.h>
#include <termios.h> 
#include <sys/socket.h>
#include <sys/stat.h>

#include "clientConstants.hpp"
#include "clientCommands.hpp"
#include "clientUtils.hpp"
#include "packetEnum.hpp"
#include "packetStruct.hpp"
#include <netinet/in.h>

using namespace std;

int getch_(void) 
{ 
    struct termios oldattr, newattr; 
    int ch; 
    tcgetattr(STDIN_FILENO, &oldattr); 
    newattr = oldattr; 
    newattr.c_lflag &= ~(ICANON | ECHO); 
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr); 
    ch = getchar(); 
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr); 
    return ch; 
}

void reverseString(char* str)
{
    if (!str) 
        return;
    int i = 0;
    int j = strlen(str) - 1;
    while (i < j) {
        char c = str[i];
        str[i] = str[j];
        str[j] = c;
        i++;
        j--;
    }
}

void showPacketClient (Packet clientPacket) {
    cout << "\n-----------------------------------------------" << endl;
    cout << "Pacote enviado com sucesso.\n" << endl;
    cout << "Tipo:" << clientPacket.type << endl;
    cout << "Tamanho do payload:" << clientPacket.payloadLength << endl;       
    cout << "Conteudo:" << clientPacket.payload << endl;
    cout << "-----------------------------------------------\n" << endl;
}

void sendPacket(int clientSocket, Packet *clientPacket) {

    uint16_t type = htons(clientPacket->type);
    uint16_t payloadLength = htons(clientPacket->payloadLength);

    size_t bufferSize = sizeof(uint16_t)*NUMBER_OF_PACKET_FIELDS + payloadLength;
    char *buffer = (char*)calloc(bufferSize,sizeof(char));

    memcpy(buffer,&type,sizeof(type));
    memcpy(buffer+sizeof(type),&payloadLength, sizeof(payloadLength));
    memcpy(buffer+sizeof(type)+sizeof(payloadLength),clientPacket->payload, payloadLength);

    send(clientSocket, buffer, bufferSize, 0);

    showPacketClient(*clientPacket);

    free(buffer);
}

void createRemoteDirectory (char username[], int clientSocket) {
    Packet clientPacket;
    clientPacket.type = HELLO;
    clientPacket.payloadLength = strlen(username)+1;
    clientPacket.payload = (char*)calloc(clientPacket.payloadLength,sizeof(char));

    strcpy(clientPacket.payload, username);
    sendPacket(clientSocket, &clientPacket);
    free(clientPacket.payload);
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
            cout << "Erro ao criar o diretorio " << client_directory_name << "para sincronizacao dos arquivos." << endl;
            exit(1);
        }     
}

void showMenu(char *argv[], int clientSocket) {

    while(true) {
        system("clear");
        cout << "PROJETO DE SISTEMAS OPERACIONAIS II" << endl;
        cout << "AUTOR: ANDREI POCHMANN KOENICH\n" << endl;
        cout << "-----------------------------------------------\n" << endl;

        cout << "Conexao com servidor estabelecida com sucesso.\n" << endl;
        cout << "USUARIO: " << argv[1] << endl;
        cout << "ENDERECO IP DO SERVIDOR: " << argv[2] << endl;
        cout << "PORTA DO SERVIDOR: " << argv[3] << endl;
        cout << "\n-----------------------------------------------\n" << endl;

        cout << "Digite o comando:" << endl;
        string command;
        getline(cin, command);

        if (command.compare(0, UPLOAD_COMMAND.length(), UPLOAD_COMMAND) == 0) {
            uploadCommandFilename(command, clientSocket);
            uploadCommandPayload(command,clientSocket);
        }
            
        /*
        else if (command.compare(0, DOWNLOAD_COMMAND.length(), DOWNLOAD_COMMAND) == 0)
            //downloadCommand(command, clientSocket);

        else if (command.compare(0, DELETE_COMMAND.length(), DELETE_COMMAND) == 0)
            //deleteCommand(command, clientSocket);

        else if (command.compare(0, LIST_SERVER_COMMAND.length(), LIST_SERVER_COMMAND) == 0)
            //listServer(command, clientSocket); 

        else if (command.compare(0, LIST_CLIENT_COMMAND.length(), LIST_CLIENT_COMMAND) == 0)
            //listClient(command, clientSocket);
        */

        else if (command.compare(0, HELP_COMMAND.length(), HELP_COMMAND) == 0)
            helpMenu();

        else if (command.compare(0, EXIT_COMMAND.length(), EXIT_COMMAND) == 0) {
            close(clientSocket);
            system("clear");
            exit(0);
        }

        else {
            cout << "Comando invalido. Pressione qualquer tecla para continuar." << endl;
            getch_();
        }
    }
}

void helpMenu() {
    system("clear");
    cout << "LISTA DE COMANDOS:" << endl;
    cout << "-----------------------------------------------\n" << endl;

    cout << "COMANDO:" << endl;
    cout << "upload <path/filename.ext>" << endl;
    cout << "DESCRICAO:" << endl;
    cout << "Envia o arquivo filename.ext para o servidor, colocando-o no “sync_dir” do  servidor e propagando-o para todos os dispositivos daquele usuario.e.g. upload /home/user/MyFolder/filename.ext\n"  << endl;

    cout << "COMANDO:" << endl;
    cout << "download <filename.ext>" << endl;
    cout << "DESCRICAO:" << endl;
    cout << "Faz uma copia nao sincronizada do arquivo filename.ext do servidor para o diretorio local (de onde o servidor foi chamado). e.g. download mySpreadsheet.xlsx\n" << endl;

    cout << "COMANDO:" << endl;
    cout << "delete <filename.ext>" << endl;
    cout << "DESCRICAO:" << endl;
    cout << "Exclui o arquivo <filename.ext> de “sync_dir”.\n"  << endl;

    cout << "COMANDO:" << endl;
    cout << "list_server" << endl;
    cout << "DESCRICAO:" << endl;
    cout << "Lista os arquivos salvos no servidor, associados ao usuario.\n"  << endl;

    cout << "COMANDO:" << endl;
    cout << "list_client" << endl;
    cout << "DESCRICAO:" << endl;
    cout << "Lista os arquivos salvos no diretorio “sync_dir”.\n"  << endl;

    cout << "COMANDO:" << endl;
    cout << "exit" << endl;
    cout << "DESCRICAO:" << endl;
    cout << "Fecha a sessao com o servidor.\n"  << endl;

    cout << "Pressione qualquer tecla para continuar." << endl;
    getch_();
}