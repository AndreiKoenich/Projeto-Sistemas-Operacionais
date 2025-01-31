#include <iostream>
#include <sys/socket.h>
#include <netinet/ip.h> 
#include <cstring>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <unistd.h>
#include <curses.h>

#include "clientConstants.hpp"
#include "clientCommands.hpp"
#include "clientUtils.hpp"
#include "inotifyClient.hpp"

using namespace std;

int serverConnection(char *argv[]) {
    int clientSocket = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(argv[2]);
    serverAddress.sin_port = htons(stoi(argv[3]));

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cout << "Erro na conexao do cliente com o servidor." << endl;
        close(clientSocket);
        exit(1);
    }
    
    return clientSocket;
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
        string username(argv[1]);
        getline(cin, command);

        if (command.compare(0, UPLOAD_COMMAND.length(), UPLOAD_COMMAND) == 0)
            uploadCommand(command, clientSocket);
    
        else if (command.compare(0, DOWNLOAD_COMMAND.length(), DOWNLOAD_COMMAND) == 0)
            requestDownloadCommand(username, command, clientSocket);
        
        else if (command.compare(0, DELETE_COMMAND.length(), DELETE_COMMAND) == 0)
            deleteCommand(username, command);

        else if (command.compare(0, LIST_SERVER_COMMAND.length(), LIST_SERVER_COMMAND) == 0)
            requestListServerCommand(username, clientSocket); 

        else if (command.compare(0, LIST_CLIENT_COMMAND.length(), LIST_CLIENT_COMMAND) == 0)
            listClientCommand(username);

        else if (command.compare(0, HELP_COMMAND.length(), HELP_COMMAND) == 0)
            helpMenu();

        else if (command.compare(0, EXIT_COMMAND.length(), EXIT_COMMAND) == 0) {
            exitCommand(clientSocket);
            exit(0);
        }

        else {
            cout << "Comando invalido. Pressione qualquer tecla para continuar." << endl;
            getch_();
        }
    }
}

int main(int argc, char *argv[]) {

    if (argc != NUMBER_OF_PARAMETERS+1) {
        cout << "Erro no formato do comando para executar o myClient. Formato correto:" << endl;
        cout << "./myClient <username> <server_ip_address> <port>" << endl;
        return 1;
    }

    pthread_t inotifyThread, menuThread;

    createClientDirectory(argv[1]);
    int clientSocket = serverConnection(argv);
    createRemoteDirectory(argv[1], clientSocket);
    monitorClientDirectory(string(argv[1]));

    showMenu(argv, clientSocket);

    close(clientSocket);
    return 0;
}