#include <iostream>
#include <arpa/inet.h>
#include <csignal>
#include <sstream>

#include "clientConstants.hpp"
#include "clientCommands.hpp"
#include "clientUtils.hpp"
#include "inotifyClient.hpp"
#include "clientPacketReceiving.hpp"
#include "packetEnum.hpp"
#include "clientPacketSending.hpp"

using namespace std;

int clientSocket;

pthread_t menuThread;
pthread_t inotifyThread;
pthread_t propagationThread;

pthread_mutex_t mutexClientDirectory = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexClientSocket = PTHREAD_MUTEX_INITIALIZER;

stringstream textToImpress;

void exitCommand(int clientSocket) {

    pthread_mutex_lock(&mutexClientSocket);

    pthread_cancel(propagationThread);
    ByePacket clientPacket;  
    clientPacket.packetType = BYE;
    sendByePacket(clientSocket, &clientPacket);
    close(clientSocket);
    pthread_cancel(inotifyThread);
    pthread_cancel(menuThread);

    pthread_mutex_unlock(&mutexClientSocket);

    exit(0);
}

void signalHandler(int signal) {
    if (signal == SIGINT) 
        exitCommand(clientSocket); 
}

int serverConnection(char *argv[]) {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(argv[2]);
    serverAddress.sin_port = htons(stoi(argv[3]));

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        textToImpress << "Erro na conexao do cliente com o servidor." << endl;
        close(clientSocket);
        exit(1);
    }

    return clientSocket;
}

void* showMenu(void* parameters) {

    clientStruct *menuParameters = (clientStruct*) parameters;

    while (true) {
        system("clear");
        cout << "PROJETO DE SISTEMAS OPERACIONAIS II" << endl;
        cout << "AUTOR: ANDREI POCHMANN KOENICH\n" << endl;
        cout << "-----------------------------------------------\n" << endl;
        cout << "Conexao com servidor estabelecida com sucesso.\n" << endl;
        cout << "USUARIO: " << menuParameters->username << endl;
        cout << "ENDERECO IP DO SERVIDOR: " << menuParameters->address << endl;
        cout << "PORTA DO SERVIDOR: " << menuParameters->port << endl;
        cout << "\n-----------------------------------------------\n" << endl;
        cout << textToImpress.str();
        textToImpress.str("");
        cout << "\nDigite o comando:" << endl;

        string command;
        getline(cin, command);

        if (command.compare(0, UPLOAD_COMMAND.length(), UPLOAD_COMMAND) == 0)
            uploadCommand(command, menuParameters);

        else if (command.compare(0, DOWNLOAD_COMMAND.length(), DOWNLOAD_COMMAND) == 0) 
            requestDownloadCommand(command, menuParameters);

        else if (command.compare(0, DELETE_COMMAND.length(), DELETE_COMMAND) == 0)
            deleteCommand(command, menuParameters);

        else if (command.compare(0, LIST_SERVER_COMMAND.length(), LIST_SERVER_COMMAND) == 0)
            requestListServerCommand(menuParameters);

        else if (command.compare(0, LIST_CLIENT_COMMAND.length(), LIST_CLIENT_COMMAND) == 0)
            listClientCommand(menuParameters);

        else if (command.compare(0, HELP_COMMAND.length(), HELP_COMMAND) == 0)
            helpMenu();

        else if (command.compare(0, EXIT_COMMAND.length(), EXIT_COMMAND) == 0)
            exitCommand(menuParameters->clientSocket);
    }

    return NULL;
}

int main(int argc, char *argv[]) {

    if (argc != NUMBER_OF_PARAMETERS+1) {
        textToImpress << "Erro no formato do comando para executar o myClient. Formato correto:" << endl;
        cout << "./myClient <username> <server_ip_address> <port>" << endl;
        exit(1);
    }

    createClientDirectory(argv[1]);
    clientSocket = serverConnection(argv);
    createRemoteDirectory(argv[1], clientSocket);

    signal(SIGINT, signalHandler);

    clientStruct menuParameters;
    menuParameters.clientSocket = clientSocket;
    menuParameters.username = argv[1];
    menuParameters.address = argv[2];
    menuParameters.port = argv[3];

    pthread_create(&inotifyThread, NULL, monitorClientDirectory, (void*)&menuParameters);
    pthread_create(&menuThread, NULL, showMenu, (void*)&menuParameters);
    pthread_create(&propagationThread, NULL, receivePacketFromServerLoop, (void*)&menuParameters);

    pthread_join(inotifyThread, NULL);
    pthread_join(menuThread, NULL);
    pthread_join(propagationThread, NULL);

    close(clientSocket);
    return 0;
}