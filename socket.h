#pragma once

#ifndef SOCKET_H
#define SOCKET_H

#ifdef _WIN32 // Windows-spezifischer Code
#include "initWinsock.h"
typedef int socklen_t;
#else // Linux-spezifischer Code
#include "linuxLib.h"
#define closesocket close
#endif

#include <string>

class SocketClss {
public:
    SocketClss();
    int sockfd;

    // Socket erstellen und mit Adresse verbinden (für Clients)
    bool C_createAndConnect(const std::string& ipAddress, int port);

    // Socket erstellen und an Port binden (für Server)
    bool S_createAndBind(int port);

    // Auf eingehende Verbindungen warten (nur für Server)
    bool S_listen(int backlog = 10);

    // Eingehende Verbindung akzeptieren (nur für Server)
    SocketClss S_acceptConnection();

    // Daten senden
    bool sendData(const std::string& message);

    // Daten empfangen
    int receiveData(char* buffer, int bufferSize);

    // Socket schließen
    void closeSocket();

private:
    
    struct sockaddr_in serverAddr;
};

#endif // MYCLASS_H
