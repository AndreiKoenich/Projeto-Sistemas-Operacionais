#include <iostream>
#include <cstring>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>

#include "clientConstants.hpp"
#include "clientUtils.hpp"
#include "clientCommands.hpp"
#include "packetEnum.hpp"
#include "clientPacketSending.hpp"
#include "clientStruct.hpp"

using namespace std;

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + NAME_MAX + 1))

void uploadInotify(string fileName, string username, int clientSocket)  {

    UploadPacket clientPacket; 

    char filePath[FULL_DIRECTORY_NAME_SIZE];
    memset(filePath,0,sizeof(filePath));
    getcwd(filePath,FULL_DIRECTORY_NAME_SIZE);
    strcat(filePath,"/");
    strcat(filePath,CLIENT_DIRECTORY_PREFIX);

    char* usernameStr = (char*)calloc(username.length()+1,sizeof(char));
    username.copy(usernameStr,username.length());
    usernameStr[username.length()] = '\0';
    strcat(filePath,usernameStr);
    strcat(filePath,"/");
    strcat(filePath,fileName.c_str());

    cout << "Arquivo selecionado pelo Inotify: " << filePath << endl;

    FILE *selectedFile;
    if((selectedFile = fopen(filePath, "rb")) == NULL) {
        cout << "Erro na abertura do arquivo para envio ao servidor, ao tentar executar o comando upload pelo Inotify." << endl;
        cout << "Pressione qualquer tecla para continuar." << endl;
        getch_();
        return;
    }

    reverseString(filePath);

    clientPacket.fileNameLength = 1;
    for (int i = 0; filePath[i] != '/'; i++)
        clientPacket.fileNameLength++;

    clientPacket.fileName = (char*)calloc(clientPacket.fileNameLength,sizeof(char));
    fileName.copy(clientPacket.fileName, clientPacket.fileNameLength-1);
    clientPacket.fileName[clientPacket.fileNameLength-1] = '\0';

    fseek(selectedFile, 0, SEEK_END);
	uint16_t fileLength=ftell(selectedFile);
	fseek(selectedFile, 0, SEEK_SET);

    clientPacket.packetType = UPLOAD_INOTIFY;
    clientPacket.payloadLength = fileLength;
	clientPacket.payload =(char*)calloc(fileLength,sizeof(char));

    if(fread(clientPacket.payload, sizeof(char), fileLength, selectedFile) != fileLength) {
        cout << "Erro na leitura do arquivo para inserir os dados em buffer, ao tentar executar o comando upload pelo Inotify." << endl;
        cout << "Pressione qualquer tecla para continuar." << endl;
        fclose(selectedFile);
        getch_();
        return;
    }

    if (fileLength != 0)
        clientPacket.payload[fileLength-1] = '\0';

    fclose(selectedFile);
    sendUploadPacket(clientSocket, &clientPacket);
    free(usernameStr);
    free(clientPacket.fileName);
    free(clientPacket.payload);
}

void deleteInotify(string fileName, int clientSocket) {

    RequestDeletePacket clientPacket;
    clientPacket.packetType = DELETE_INOTIFY;
    clientPacket.fileNameLength = fileName.length()+1;
    clientPacket.fileName = (char*)calloc(clientPacket.fileNameLength,sizeof(char));
    fileName.copy(clientPacket.fileName,clientPacket.fileNameLength-1);
    clientPacket.fileName[clientPacket.fileNameLength-1] = '\0';

    sendRequestDeletePacket(clientSocket, &clientPacket);
    cout << "Comando de requisicao de delete executado com sucesso." << endl;

    free(clientPacket.fileName);
}

bool isTemporaryFile(const string fileName) {
    return fileName.front() == '~' || fileName.front() == '.';
}

void* monitorClientDirectory (void* parameters) {

    clientStruct *menuParameters;
    menuParameters = (clientStruct*) parameters;

    string username((char*)menuParameters->username);

    char clientDirectory[FULL_DIRECTORY_NAME_SIZE];
    memset(clientDirectory,0,sizeof(clientDirectory));
    getcwd(clientDirectory,FULL_DIRECTORY_NAME_SIZE);
    strcat(clientDirectory,"/");
    strcat(clientDirectory,CLIENT_DIRECTORY_PREFIX);

    char* usernameStr = (char*)calloc(username.length()+1,sizeof(char));
    username.copy(usernameStr,username.length());
    usernameStr[username.length()] = '\0';
    strcat(clientDirectory,usernameStr);
    strcat(clientDirectory,"/");
    free(usernameStr);

    int inotify_fd = inotify_init();
    if (inotify_fd < 0) {
        cerr << "Erro ao inicializar inotify." << endl;
        return NULL;
    }

    int watch_descriptor = inotify_add_watch(inotify_fd, clientDirectory, IN_MOVED_FROM | IN_MOVED_TO | IN_CLOSE_WRITE | IN_CREATE);
    
    if (watch_descriptor < 0) {
        cerr << "Erro ao adicionar watch ao diretório." << endl;
        close(inotify_fd);
        return NULL;
    }

    char buffer[BUF_LEN];
    cout << "Monitorando o diretório: " << clientDirectory << endl;

    string fileName, lastModifiedFile;
    bool modificationFlag = false;

    while (true) {
        int length = read(inotify_fd, buffer, BUF_LEN);
        if (length < 0) {
            cerr << "Erro ao ler eventos do inotify." << endl;
            break;
        }

        for (int i = 0; i < length;) {
                    struct inotify_event *event = (struct inotify_event *) &buffer[i];
                    if (event->len) {
                        string fileName = event->name;
                        
                        if (!isTemporaryFile(fileName))
                        {   
                            if (event->mask & IN_MOVED_TO || event->mask & IN_CREATE) {

                                if (lastModifiedFile == fileName) 
                                    modificationFlag = true;
                                else {
                                    cout << "Arquivo " << fileName << " foi movido para dentro do diretorio." << endl;
                                    uploadInotify(fileName, menuParameters->username, menuParameters->clientSocket);

                                }     
                            }
                        
                            else if (event->mask & IN_CLOSE_WRITE) {
                                lastModifiedFile = fileName;

                                if (modificationFlag == true) {
                                    cout << "Arquivo " << fileName << " foi modificado." << endl;
                                    modificationFlag = false;
                                    uploadInotify(fileName, menuParameters->username, menuParameters->clientSocket);
                                }   
                            }

                            else if (event->mask & IN_MOVED_FROM) {
                                cout << "Arquivo " << fileName << " foi movido para fora do diretorio." << endl;
                                deleteInotify(fileName, menuParameters->clientSocket);
                            }
                                    
                        
                    }
                    i += EVENT_SIZE + event->len;
                }
        }
    }

    inotify_rm_watch(inotify_fd, watch_descriptor);
    close(inotify_fd);

    return NULL;
}