#ifdef _WIN32 // Windows-spezifischer Code
#include "initWinsock.h"
typedef int socklen_t;
#else // Linux-spezifischer Code
#include "linuxLib.h"
#define closesocket close
#endif


#include <iostream>


class ListeningSocket {
public:
    ListeningSocket(int port) : port(port), sockfd(-1) {}

    bool createAndBind() {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            //std::cerr << "Fehler beim Erstellen des Sockets: " << strerror(errno) << std::endl;
            return false;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY; // Auf allen verfügbaren Interfaces lauschen
        serverAddr.sin_port = htons(port);

        if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            //std::cerr << "Fehler beim Binden des Sockets: " << strerror(errno) << std::endl;
            closeSocket(); // Socket schließen bei Fehler
            return false;
        }
        return true;
    }

    bool listenForConnections(int backlog = 10) {
        if (listen(sockfd, backlog) < 0) { // backlog: Anzahl der wartenden Verbindungen
            //std::cerr << "Fehler beim 'listen': " << strerror(errno) << std::endl;
            return false;
        }
        return true;
    }

    int acceptConnection() {
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket < 0) {
            //std::cerr << "Fehler beim 'accept': " << strerror(errno) << std::endl;
        }
        return clientSocket; // Neuen Socket-Deskriptor für die Client-Verbindung zurückgeben
    }

    void closeSocket() {
        if (sockfd >= 0) { // Nur schließen, wenn der Socket gültig ist
#ifdef _WIN32
            closesocket(sockfd);
#else
            close(sockfd);
#endif
            sockfd = -1; // Socket-Deskriptor auf ungültig setzen
        }
    }

private:
    int port;
    int sockfd;
    struct sockaddr_in serverAddr;
};