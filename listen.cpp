#ifdef _WIN32 // Windows-spezifischer Code
#include "initWinsock.h"
#pragma comment(lib, "Ws2_32.lib")
typedef int socklen_t;
#else // Linux-spezifischer Code
#include "linuxLib.h"
#define closesocket close
#endif


#include <sstream>
#include <iostream>
#include <string.h> // Include the string.h header


#include "connect.h"
#include "DataStorage.h"
#include "socket.h"

//close  -  closesocketsocket
#define PORT 26000


using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

SocketClss HandleFirstHandshake(SocketClss originalSocket,int Port, double OwnVersion) {
    if(!(originalSocket.sockfd >= 0)){

        
        if (!originalSocket.S_createAndBind(Port)) {
            std::cerr << "Fehler beim Erstellen oder Binden des Sockets!" << std::endl;
            return SocketClss(); // Rückgabe eines leeren MySocket-Objekts im Fehlerfall
        }

        if (!originalSocket.S_listen()) { // Auf Verbindungen warten
            std::cerr << "Fehler beim 'listen': " << std::endl;
            originalSocket.closeSocket();
            return SocketClss();
        }
    }

    SocketClss acceptSocket = originalSocket.S_acceptConnection(); // Verwende acceptConnection von MySocket
    if (acceptSocket.sockfd < 0) {
        std::cerr << "Fehler beim 'accept': " << std::endl;
        originalSocket.closeSocket();
        return SocketClss();
    }

    char dataBuffer[1024] = { 0 };
    int receiveData = acceptSocket.receiveData(dataBuffer, 1024);  // Verwende receiveData von MySocket

    if (receiveData <= 0) {
        std::cerr << "Fehler beim Empfangen von Daten." << std::endl;
        acceptSocket.closeSocket();
        originalSocket.closeSocket();
        return SocketClss();
    }

    std::string connectResponse(dataBuffer, 14);
    if (connectResponse == "INFO2 CONNECT/") {

        std::cout << "received Connection attempt" << std::endl;
        std::stringstream ss2;
        std::string responseVers(dataBuffer + 14);
        ss2 << responseVers;
        double clientVersion;
        ss2 >> clientVersion;

        if (clientVersion >= OwnVersion) {
            std::cout << "handshake successful\n";
            std::string acceptConnection = "INFO2 OK\n\n";

            acceptSocket.sendData(acceptConnection); // Verwende sendData von MySocket
            std::cout << "responds with INFO2 OK! " << std::endl;

            acceptSocket.closeSocket(); // Schließe den Server-Socket nach dem Handshake
            return acceptSocket;                            // Gib den Client-Socket zurück
        }
        else {
            std::cout << "handshake failed -> old version: " << clientVersion << endl;
        }
        std::cout << "handshake failed\n";
    }

    acceptSocket.closeSocket(); // Schließe den Server-Socket nach dem Handshake
    return originalSocket; // Gib den Client-Socket zurück
}

int listenForIncomingConnection(SocketClss& socket, string ownIP, double OwnVersion, vector<SocketClss> &IPStr, vector<int> &MessageIDs)
{

    cout << "Warte auf Verbindungen..." << endl;

    // Unendliche Schleife, um mehrere Client-Verbindungen zu akzeptieren
    while (true)
    {
        if (socket.sockfd == -1)
        {
            cout<< "k" << endl;
            continue; // Zum nächsten Schleifendurchlauf springen
        }
        else
        {
           
            // 6. recieve data
            char dataBuffer[1024] = {0};
            int recieveData = socket.receiveData(dataBuffer, 1024);
            if (recieveData > 0){
                cout << "daten Erhalten" << dataBuffer << endl;
                continue;
            }
            {
                /* code */
            }
            

            string BackconnectResponse (dataBuffer, 11);
            if (!(strcmp(BackconnectResponse.c_str(), "BACKCONNECT"))) {
                cout << "receved backConnection attempt" << endl;
                string responseIP(dataBuffer + 12);
                cout << responseIP;
                firstHandshake(responseIP, PORT ,OwnVersion);    
            }
            
            string FriendRqResponse(dataBuffer, 18);
            if (!(strcmp(BackconnectResponse.c_str(), "FRIEND REQUEST\n\n"))) {
                
                string IP;
                IP = giveIP(IPStr);
                socket.sendData(IP);
            }

            string SENDResponse(dataBuffer, 4);
            if (!(strcmp(SENDResponse.c_str(), "SEND"))) {
                
                string MessageToForward(dataBuffer);
                string RecevedMessageID (dataBuffer + 5, 11);
                std::stringstream ss3;
                ss3 << RecevedMessageID;
                int RecevedMessageIDint;
                ss3 >> RecevedMessageIDint;

                if (!checkMessageID(MessageIDs, RecevedMessageIDint)) {
                    sendMessage(MessageToForward, RecevedMessageIDint, IPStr);
                }

                
            }

        }
    } // Ende der Schleife

    // Server-Socket schließen (wird nie erreicht)
  
    socket.closeSocket();
    return 0;
} 
