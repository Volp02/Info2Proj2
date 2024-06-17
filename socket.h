#pragma once
#ifndef SOCKET_H
#define SOCKET_H

#ifdef _WIN32 // Windows-spezifischer Code
#include "initWinsock.h"
typedef int socklen_t;
#else // Linux-spezifischer Code
#include "linuxLib.h"
#include <cstring> // f�r strerror() und errno
#define closesocket close
#endif

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class SocketClss {
public:
    SocketClss() : sockfd(-1) {}
#ifdef _WIN32
    SOCKET sockfd;
#else
    int sockfd;
#endif
    string ipAddress = "0.0.0.0";

    //retutns socket file descriptor
    int getSocket(){
        return sockfd;
    }
    // Socket erstellen und mit Adresse verbinden (f�r Clients)
    bool C_createAndConnect(const string& ipAddress, int port) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {

            return false;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        if (inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr) <= 0) {
            cerr << "Ung�ltige Adresse oder Adresse nicht unterst�tzt\n";
            return false;
        }

        if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {

            return false;
        }
        return true;
    }

    // Socket erstellen und an Port binden (für Server)
    bool S_createAndBind(int port) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {

            return false;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY; // Auf allen verfügbaren Interfaces lauschen
        serverAddr.sin_port = htons(port);

        if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {

            closeSocket(); // Socket schlie�en bei Fehler
            return false;
        }
        return true;
    }

    // Auf eingehende Verbindungen warten (nur für Server)
    bool S_listen(int backlog = 2) {
        listen(sockfd, backlog);
        return true;
    }

    // Eingehende Verbindung akzeptieren (nur für Server)
    SocketClss* S_acceptConnection() {
        SocketClss* acceptSocket = new SocketClss();                    // Allocate on heap
        socklen_t clientAddrLen = sizeof(acceptSocket->serverAddr);
        acceptSocket->sockfd = accept(sockfd, (struct sockaddr*)&acceptSocket->serverAddr, &clientAddrLen);

        if (acceptSocket->sockfd == -1) {
            delete acceptSocket;                                        // Clean up if accept failed
            return nullptr;
        }

        return acceptSocket;                                            // Return the pointer to the dynamically allocated object
    }

    // Daten senden
    bool sendData(const string& message) {

        if (send(sockfd, message.c_str(), message.length(), 0) < 0) {
            cerr << "Fehler beim Senden der Daten: " << message << endl;
            return false;
        }
        //DEBUG
        cout << "data send: " << message << endl;
        return true;
    }

    // Daten empfangen
    int receiveData(char* buffer, int bufferSize) {
        //buffer leeren:
        memset(buffer, 0, bufferSize);
        return recv(sockfd, buffer, bufferSize, 0);
    }

    // Socket schließen
    void closeSocket() {
        if (sockfd >= 0) { // Nur schlie�en, wenn der Socket g�ltig ist

            //cout<< "Closing socket..." << sockfd <<endl;
            closesocket(sockfd);

            sockfd = -1; // Socket-Deskriptor auf ung�ltig setzen
        }
    }

    // Handshake mit Server (returns true if handshake was successful)
    bool handshakeOut(double Version){
        
        //convert to string with precision 1 (eine Nachkommastelle)
        stringstream versionSS;
        versionSS.precision(1);
        versionSS << fixed << Version; // Beachten Sie die Verwendung von "version" (Kleinbuchstaben)
        string stringVersion = versionSS.str();


        this->sendData("INFO2 CONNECT/"+stringVersion+"\n\n");
        char buffer[1024];
        this->receiveData(buffer, 1024);

        string HSresponse(buffer);
        cout << "Handshake response: " << HSresponse << endl;
        if(HSresponse =="INFO2 OK\n\n"){
            return true;
        }
        else{
            return false;
        }


    }


    string handshakeIn(char dataBuffer[], double OwnVersion)
{
    string connectResponse(dataBuffer, 14);
    if (connectResponse == "INFO2 CONNECT/")
    {

        cout << "received Connection attempt" << endl;
        stringstream ss2;
        string responseVers(dataBuffer + 14);
        ss2 << responseVers;
        double clientVersion;
        ss2 >> clientVersion;

        if (clientVersion >= OwnVersion)
        {
            cout << "incoming handshake successful\n";
            return "INFO2 OK\n\n";
        }
        else
        {

            cout << "handshake failed -> old version: " << clientVersion << endl;
            return "handshake failed: old version!\nYour version: " + to_string(clientVersion) + " Required version: " + to_string(OwnVersion) + " or higher\n\n";
        }
    }

    return "Handshake failed!\n   - bad request\n\n";
}


private:

    struct sockaddr_in serverAddr;
};



#endif // MYCLASS_H
