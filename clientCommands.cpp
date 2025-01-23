#include <iostream>
#include <sys/socket.h>
#include <bits/stdc++.h>

#include "clientConstants.hpp"
#include "clientUtils.hpp"
#include "clientCommands.hpp"
#include "packetStruct.hpp"
#include "packetEnum.hpp"

void uploadCommandFilename(string command, int clientSocket) {

    string filePath = command.substr(UPLOAD_COMMAND.length(), command.length() - UPLOAD_COMMAND.length());
    Packet clientPacket;

    clientPacket.type = UPLOAD_FILENAME;
    reverse(filePath.begin(),filePath.end());

    clientPacket.payloadLength = 1;
    for (int i = 0; filePath[i] != '/'; i++)
        clientPacket.payloadLength++;

    clientPacket.payload=(char*)calloc(clientPacket.payloadLength,sizeof(char));
    
    filePath.copy(clientPacket.payload, clientPacket.payloadLength-1);
    clientPacket.payload[clientPacket.payloadLength-1] = '\0';
    reverseString(clientPacket.payload);

    sendPacket(clientSocket, &clientPacket);
    free(clientPacket.payload);
}

void uploadCommandPayload(string command, int clientSocket) {

    string filePath = command.substr(UPLOAD_COMMAND.length(), command.length() - UPLOAD_COMMAND.length());
    Packet clientPacket;
    
    FILE *selectedFile;
    if((selectedFile = fopen(filePath.data(), "rb")) == NULL) {
        cout << "Erro na abertura do arquivo para envio ao servidor, ao tentar executar o comando upload." << endl;
        cout << "Pressione qualquer tecla para continuar." << endl;
        getch_();
        return;
    }

    fseek(selectedFile, 0, SEEK_END);
	uint16_t fileLength=ftell(selectedFile);
	fseek(selectedFile, 0, SEEK_SET);

    clientPacket.type = UPLOAD_PAYLOAD;
    clientPacket.payloadLength = fileLength;
	clientPacket.payload =(char*)calloc(fileLength,sizeof(char));

    if(fread(clientPacket.payload, sizeof(char), fileLength, selectedFile) != fileLength) {
        cout << "Erro na leitura do arquivo para inserir os dados em buffer, ao tentar executar o comando upload." << endl;
        cout << "Pressione qualquer tecla para continuar." << endl;
        fclose(selectedFile);
        getch_();
        return;
    }

    fclose(selectedFile);
    sendPacket(clientSocket, &clientPacket);
    free(clientPacket.payload);
    cout << "Comando upload executado com sucesso. Pressione qualquer tecla para continuar." << endl;
    getch_();
}