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

int main(int argc, char *argv[]) {
    if (argc != NUMBER_OF_PARAMETERS+1) {
        cout << "Erro no formato do comando para executar o myClient. Formato correto:" << endl;
        cout << "./myClient <username> <server_ip_address> <port>" << endl;
        return 1;
    }

    createClientDirectory(argv[1]);
    int clientSocket = serverConnection(argv);
    createRemoteDirectory(argv[1], clientSocket);
    showMenu(argv, clientSocket);

    close(clientSocket);
    return 0;
}