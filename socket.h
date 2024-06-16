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

    // Socket erstellen und mit Adresse verbinden (f�r Clients)
    bool C_createAndConnect(const std::string& ipAddress, int port);

    // Socket erstellen und an Port binden (f�r Server)
    bool S_createAndBind(int port);

    // Auf eingehende Verbindungen warten (nur f�r Server)
    bool S_listen(int backlog = 10);

    // Eingehende Verbindung akzeptieren (nur f�r Server)
    SocketClss S_acceptConnection();

    // Daten senden
    bool sendData(const std::string& message);

    // Daten empfangen
    int receiveData(char* buffer, int bufferSize);

    // Socket schlie�en
    void closeSocket();

private:
    
    struct sockaddr_in serverAddr;
};

#endif // MYCLASS_H
