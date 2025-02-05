#include <iostream>
#include <netinet/in.h>

#include "clientConstants.hpp"
#include "clientCommands.hpp"
#include "clientUtils.hpp"
#include "packetEnum.hpp"
#include "packetStruct.hpp"

void sendRequestDownloadPacket(int clientSocket, RequestDownloadPacket *clientPacket) {
    uint16_t packetType = htons(clientPacket->packetType);
    uint16_t fileNameLength = htons(clientPacket->fileNameLength);

    size_t bufferSize = sizeof(uint16_t)*NUMBER_OF_REQUEST_DOWNLOAD_PACKET_FIELDS + clientPacket->fileNameLength;
    char *buffer = (char*)calloc(bufferSize,sizeof(char));
    memcpy(buffer,&packetType,sizeof(packetType));
    memcpy(buffer+sizeof(packetType),&fileNameLength, sizeof(clientPacket->fileNameLength));
    memcpy(buffer+sizeof(packetType)+sizeof(fileNameLength),clientPacket->fileName, clientPacket->fileNameLength);

    send(clientSocket, buffer, bufferSize, 0);

    showRequestDownloadPacketClient(*clientPacket);

    free(buffer); 
}

void sendUploadPacket(int clientSocket, UploadPacket *clientPacket) {

    uint16_t packetType = htons(clientPacket->packetType);
    uint16_t payloadLength = htons(clientPacket->payloadLength);
    uint16_t fileNameLength = htons(clientPacket->fileNameLength);

    size_t bufferSize = sizeof(uint16_t)*NUMBER_OF_UPLOAD_PACKET_FIELDS + clientPacket->payloadLength + clientPacket->fileNameLength;
    char *buffer = (char*)calloc(bufferSize,sizeof(char));

    memcpy(buffer,&packetType,sizeof(packetType));
    memcpy(buffer+sizeof(packetType),&fileNameLength, sizeof(clientPacket->fileNameLength));
    memcpy(buffer+sizeof(packetType)+sizeof(fileNameLength),clientPacket->fileName, clientPacket->fileNameLength);
    memcpy(buffer+sizeof(packetType)+sizeof(fileNameLength)+clientPacket->fileNameLength, &payloadLength, sizeof(clientPacket->payloadLength));
    memcpy(buffer+sizeof(packetType)+sizeof(fileNameLength)+clientPacket->fileNameLength+sizeof(clientPacket->payloadLength), clientPacket->payload, clientPacket->payloadLength);

    send(clientSocket, buffer, bufferSize, 0);

    showUploadPacketClient(*clientPacket);

    free(buffer);
}

void sendHelloPacket(int clientSocket, HelloPacket *clientPacket) {

    uint16_t packetType = htons(clientPacket->packetType);
    uint16_t usernameLength = htons(clientPacket->usernameLength);

    size_t bufferSize = sizeof(uint16_t)*NUMBER_OF_HELLO_PACKET_FIELDS + clientPacket->usernameLength;
    char *buffer = (char*)calloc(bufferSize,sizeof(char));

    memcpy(buffer,&packetType,sizeof(packetType));
    memcpy(buffer+sizeof(packetType),&usernameLength, sizeof(clientPacket->usernameLength));
    memcpy(buffer+sizeof(packetType)+sizeof(usernameLength),clientPacket->username, clientPacket->usernameLength);

    send(clientSocket, buffer, bufferSize, 0);

    showHelloPacketClient(*clientPacket);

    free(buffer);
}

void sendByePacket(int clientSocket, ByePacket *clientPacket) {

    uint16_t packetType = htons(clientPacket->packetType);

    size_t bufferSize = sizeof(uint16_t)*NUMBER_OF_BYE_PACKET_FIELDS;
    char *buffer = (char*)calloc(bufferSize,sizeof(char));

    memcpy(buffer,&packetType,sizeof(packetType));
    send(clientSocket, buffer, bufferSize, 0);

    showByePacketClient(*clientPacket);

    free(buffer);
}

void sendRequestListServerPacket (int clientSocket, RequestListServerPacket *clientPacket) {

    uint16_t packetType = htons(clientPacket->packetType);

    size_t bufferSize = sizeof(uint16_t)*NUMBER_OF_REQUEST_LIST_SERVER_PACKET_FIELDS;
    char *buffer = (char*)calloc(bufferSize,sizeof(char));
    memcpy(buffer,&packetType,sizeof(packetType));
    send(clientSocket, buffer, bufferSize, 0);

    showRequestListServerPacketClient(*clientPacket);

    free(buffer);
}

void sendRequestDeletePacket(int clientSocket, RequestDeletePacket *clientPacket) {
    uint16_t packetType = htons(clientPacket->packetType);
    uint16_t fileNameLength = htons(clientPacket->fileNameLength);

    size_t bufferSize = sizeof(uint16_t)*NUMBER_OF_DELETE_INOTIFY_PACKET_FIELDS + clientPacket->fileNameLength;
    char *buffer = (char*)calloc(bufferSize,sizeof(char));
    memcpy(buffer,&packetType,sizeof(packetType));
    memcpy(buffer+sizeof(packetType),&fileNameLength, sizeof(clientPacket->fileNameLength));
    memcpy(buffer+sizeof(packetType)+sizeof(fileNameLength),clientPacket->fileName, clientPacket->fileNameLength);

    send(clientSocket, buffer, bufferSize, 0);

    showRequestDeletePacketClient(*clientPacket);

    free(buffer); 
}