#include <bits/stdc++.h>
#include <netinet/in.h>

#include "serverConstants.hpp"
#include "serverUtils.hpp"

void sendDownloadPacket(int clientSocket, DownloadPacket *clientPacket) {
    
    uint16_t packetType = htons(clientPacket->packetType);
    uint16_t payloadLength = htons(clientPacket->payloadLength);
    uint16_t fileNameLength = htons(clientPacket->fileNameLength);

    size_t bufferSize = sizeof(uint16_t)*NUMBER_OF_DOWNLOAD_PACKET_FIELDS + clientPacket->payloadLength + clientPacket->fileNameLength;
    char *buffer = (char*)calloc(bufferSize,sizeof(char));

    memcpy(buffer,&packetType,sizeof(packetType));
    memcpy(buffer+sizeof(packetType),&fileNameLength, sizeof(clientPacket->fileNameLength));
    memcpy(buffer+sizeof(packetType)+sizeof(fileNameLength),clientPacket->fileName, clientPacket->fileNameLength);
    memcpy(buffer+sizeof(packetType)+sizeof(fileNameLength)+clientPacket->fileNameLength, &payloadLength, sizeof(clientPacket->payloadLength));
    memcpy(buffer+sizeof(packetType)+sizeof(fileNameLength)+clientPacket->fileNameLength+sizeof(clientPacket->payloadLength), clientPacket->payload, clientPacket->payloadLength);

    send(clientSocket, buffer, bufferSize, 0);

    //showDownloadPacketServer(*clientPacket);

    free(buffer);
}

void sendDownloadErrorPacket(int clientSocket, DownloadErrorPacket *clientPacket) {

    uint16_t packetType = htons(clientPacket->packetType);
    size_t bufferSize = sizeof(uint16_t)*NUMBER_OF_DOWNLOAD_ERROR_PACKET_FIELDS;
    char *buffer = (char*)calloc(bufferSize,sizeof(char));

    memcpy(buffer,&packetType,sizeof(packetType));
    send(clientSocket, buffer, bufferSize, 0);
    //showDownloadErrorPacketServer();
    free(buffer);
}

void sendListServerPacket(int clientSocket, ListServerPacket *clientPacket) {

    uint16_t packetType = htons(clientPacket->packetType);
    uint16_t payloadLength = htons(clientPacket->payloadLength);

    size_t bufferSize = sizeof(uint16_t)*NUMBER_OF_LIST_SERVER_PACKET_FIELDS + clientPacket->payloadLength;
    char *buffer = (char*)calloc(bufferSize,sizeof(char));

    memcpy(buffer,&packetType,sizeof(packetType));
    memcpy(buffer+sizeof(packetType),&payloadLength, sizeof(payloadLength));
    memcpy(buffer+sizeof(packetType)+sizeof(payloadLength), clientPacket->payload, clientPacket->payloadLength);
    
    send(clientSocket, buffer, bufferSize, 0);
    //showListServerPacket(*clientPacket);
    free(buffer);
}