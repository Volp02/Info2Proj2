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

// close  -  closesocketsocket
#define PORT 26000

using namespace std;
/*
SocketClss firstHandshakeHandler(string ownIP, double OwnVersion)

{
    cout << "--- im thread firstHandshakeHandler" << endl;
    SocketClss ServerSocket;
    ServerSocket.S_createAndBind(PORT); // create and bind socke

    // DEBUG
    cout << "serverSocket created and bound" << endl;

    ServerSocket.S_listen(PORT);
    cout << "Warte auf Verbindungen..." << endl;

    // Unendliche Schleife, um mehrere Client-Verbindungen zu akzeptieren
    while (true)
    {

        SocketClss acceptSocket = ServerSocket.S_acceptConnection();
        // 6. recieve data
        char dataBuffer[1024] = {0};
        // recv(acceptSocket.getSocket(), dataBuffer, 1024, 0);
        int recieveData = acceptSocket.receiveData(dataBuffer, 1024);
        if (recieveData > 0)
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
                    cout << "handshake successful\n";
                    string acceptConnection = "INFO2 OK\n\n";

                    acceptSocket.sendData(acceptConnection); // Verwende sendData von MySocket
                    cout << "responds with INFO2 OK! " << endl;

                    return acceptSocket;
                }
                else
                {
                    cout << "handshake failed -> old version: " << clientVersion << endl;
                    return SocketClss();
                }
            }
        }

        // ServerSocket.S_listen(PORT);
    }
}
*/

int listenHandler(SocketClss &socket, string ownIP, double OwnVersion, vector<string> &knownClients, vector<int> &MessageIDs)
{
    cout << "--- im thread listenForIncomingConnection" << endl;
    socket.S_createAndBind(PORT); // create and bind socke
    
    
    cout << "Warte auf Verbindungen..." << endl;
    // 5. accept connection

    socket.S_listen(PORT);
    socket.sockfd = -1 ;
    SocketClss acceptSocket = socket.S_acceptConnection();

    // 6. recieve data

    // Unendliche Schleife, um mehrere Client-Verbinungen zu akzeptieren
    while (acceptSocket.sockfd >= 0)
    {   
        
        char dataBuffer[1024] = {0};

        acceptSocket.receiveData(dataBuffer, 1024); 

        cout << "data recieved: " << dataBuffer << endl;

        string connectResponse(dataBuffer);
        //cout << "dataBuffer string " << connectResponse << endl;

        if (sizeof(dataBuffer) > 0)
        { // check handshake response

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
                    socket.sendData("INFO2 OK\n\n");
                    cout << "responds with INFO2 OK! " << endl;

                    acceptSocket.closeSocket(); // Schließe den Server-Socket nach dem Handshake
                }
                else
                {
                    cout << "handshake failed -> old version: " << clientVersion << endl;
                    acceptSocket.closeSocket(); // Schließe den Server-Socket nach einem fehlenden Handshake
                }
            }

            string BackconnectResponse(dataBuffer, 11);
            if (!(strcmp(BackconnectResponse.c_str(), "BACKCONNECT")))
            {
                cout << "receved backConnection attempt" << endl;
                string responseIP(dataBuffer + 12);
                cout << responseIP;
                if(checkIP(knownClients, responseIP)){
                    firstHandshake(responseIP, PORT, OwnVersion, knownClients);
                }
                else cout<< responseIP <<" is already a known client" << endl;
                
            }

            string FriendRqResponse(dataBuffer, 18);
            if (!(strcmp(BackconnectResponse.c_str(), "FRIEND REQUEST\n\n")))
            {
                string IP;
                IP = giveIP(knownClients);
                socket.sendData(IP);
            }

            string SENDResponse(dataBuffer, 4);
            if (!(strcmp(SENDResponse.c_str(), "SEND")))
            {

                string MessageToForward(dataBuffer);
                string RecevedMessageID(dataBuffer + 5, 11);
                stringstream ss3;
                ss3 << RecevedMessageID;
                int RecevedMessageIDint;
                ss3 >> RecevedMessageIDint;

                if (!checkMessageID(MessageIDs, RecevedMessageIDint))       //Check if messageID is already known
                {
                    storeMessageID(MessageIDs, RecevedMessageIDint);        //Adds messageID to already known messagIDs
                    sendMessageToClients(MessageToForward,  RecevedMessageIDint, knownClients, PORT, OwnVersion);   //forwars message to all clients
                }
                
            }
        }
        else
            break;

        // cout << "whiling";

    } // Ende der Schleife

    // Server-Socket schließen (wird nie erreicht)

    socket.closeSocket();

    cout << "the thread listenForIncomingConnection has finished" << endl;
    return 0;
}


bool listenThread(){

     cout << "--- im thread listenForIncomingConnection" << endl;
    
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};


    // 1. Socket erstellen
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cout << "socket failed" << std::endl;
        return 1;
    }


    // 2. Socket-Adresse konfigurieren
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);     // Port setzen

    // 3. Socket an Port binden
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cout << "bind failed" << std::endl;
        return 1;
    }

    // 4. Auf Verbindungen warten (listen)
    if (listen(server_fd, 5) < 0) {
        std::cout << "listen failed" << std::endl;
        return 1;
    }
    for(int i = 0; i < 5; i++){

        std::cout << "Server wartet auf Verbindungen auf " << "127.0.0.1" << ":" << PORT << std::endl;

        // 5. Verbindung annehmen (accept)
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            std::cout << "accept failed" << std::endl;
            return 1;
        }

        std::cout << "Verbindung akzeptiert!" << std::endl;
        
        // 6. Daten empfangen (recv)
        int valread = read(new_socket, buffer, 1024);
        std::cout << "Empfangene Nachricht: " << buffer << std::endl << std::endl;

        memset(buffer, 0, 1024);



    }

}