#pragma once
#ifndef SOCKET_H
#define SOCKET_H

#ifdef _WIN32 // Windows-spezifischer Code
#include "initWinsock.h"
typedef int socklen_t;
#else // Linux-spezifischer Code
#include "linuxLib.h"
#include <cstring> // für strerror() und errno
#define closesocket close
#endif

#include <string>
#include <iostream>

class SocketClss {
public:
    SocketClss() : sockfd(-1) {}

    int sockfd;
    // Socket erstellen und mit Adresse verbinden (für Clients)
    bool C_createAndConnect(const std::string& ipAddress, int port) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
#ifdef _WIN32
            char buffer[256]; // Angemessene Puffergröße
            strerror_s(buffer, sizeof(buffer), errno);
            std::cerr << "Fehler beim Erstellen des Sockets: " << buffer << std::endl;
#else
            std::cerr << "Fehler beim Erstellen des Sockets: " << strerror(errno) << std::endl;
#endif
            return false;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        if (inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr) <= 0) {
            std::cerr << "Ungültige Adresse oder Adresse nicht unterstützt\n";
            return false;
        }

        if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
#ifdef _WIN32
            char buffer[256]; // Angemessene Puffergröße
            strerror_s(buffer, sizeof(buffer), errno);
            std::cerr << "Verbindungsfehler: " << buffer << std::endl;
#else
            std::cerr << "Verbindungsfehler: " << strerror(errno) << std::endl;
#endif
            return false;
        }
        return true;
    }

    // Socket erstellen und an Port binden (für Server)
    bool S_createAndBind(int port) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
#ifdef _WIN32
            char buffer[256]; // Angemessene Puffergröße
            strerror_s(buffer, sizeof(buffer), errno);
            std::cerr << "Fehler beim Erstellen des Sockets: " << buffer << std::endl;
#else
            std::cerr << "Fehler beim Erstellen des Sockets: " << strerror(errno) << std::endl;
#endif
            return false;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY; // Auf allen verfügbaren Interfaces lauschen
        serverAddr.sin_port = htons(port);

        if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
#ifdef _WIN32
            char buffer[256]; // Angemessene Puffergröße
            strerror_s(buffer, sizeof(buffer), errno);
            std::cerr << "Fehler beim Binden des Sockets: " << buffer << std::endl;
#else
            std::cerr << "Fehler beim Binden des Sockets: " << strerror(errno) << std::endl;
#endif
            closeSocket(); // Socket schließen bei Fehler
            return false;
        }
        return true;
    }

    // Auf eingehende Verbindungen warten (nur für Server)
    bool S_listen(int backlog = 10) {
        if (::listen(sockfd, backlog) < 0) { // backlog: Anzahl der wartenden Verbindungen
#ifdef _WIN32
            char buffer[256]; // Angemessene Puffergröße
            strerror_s(buffer, sizeof(buffer), errno);
            std::cerr << "Fehler beim listening des Sockets: " << buffer << std::endl;
#else
            std::cerr << "Fehler beim listening des Sockets: " << strerror(errno) << std::endl;
#endif
            return false;
        }
        return true;
    }

    // Eingehende Verbindung akzeptieren (nur für Server)
    SocketClss S_acceptConnection() {
        SocketClss acceptSocket;
        socklen_t clientAddrLen = sizeof(acceptSocket.serverAddr);
        acceptSocket.sockfd = accept(sockfd, (struct sockaddr*)&acceptSocket.serverAddr, &clientAddrLen);
        if (acceptSocket.sockfd < 0) {
#ifdef _WIN32
            char buffer[256]; // Angemessene Puffergröße
            strerror_s(buffer, sizeof(buffer), errno);
            std::cerr << "Fehler beim accept des Sockets: " << buffer << std::endl;
#else
            std::cerr << "Fehler beim accept des Sockets: " << strerror(errno) << std::endl;
#endif
        }
        return acceptSocket; // Neues MySocket-Objekt für die Client-Verbindung zurückgeben
    }

    // Daten senden
    bool sendData(const std::string& message) {
        if (send(sockfd, message.c_str(), message.length(), 0) < 0) {
#ifdef _WIN32
            char buffer[256]; // Angemessene Puffergröße
            strerror_s(buffer, sizeof(buffer), errno);
            std::cerr << "Fehler beim senden der Nachricht: " << buffer << std::endl;
#else
            std::cerr << "Fehler beim senden der Nachricht: " << strerror(errno) << std::endl;
#endif
            return false;
        }
        return true;
    }

    // Daten empfangen
    int receiveData(char* buffer, int bufferSize) {
        return recv(sockfd, buffer, bufferSize, 0);
    }

    // Socket schließen
    void closeSocket() {
        if (sockfd >= 0) { // Nur schließen, wenn der Socket gültig ist

            closesocket(sockfd);

            sockfd = -1; // Socket-Deskriptor auf ungültig setzen
        }
    }

private:

    struct sockaddr_in serverAddr;
};



#endif // MYCLASS_H
