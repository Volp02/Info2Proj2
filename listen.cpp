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

int listenHandler(string ownIP, double OwnVersion, vector<string> &knownClients, vector<int> &MessageIDs, int threadNum)
{

    SocketClss serverSocket;
    serverSocket.S_createAndBind(PORT);
    serverSocket.S_listen();

    SocketClss acceptSocket;
    acceptSocket = serverSocket.S_acceptConnection();

    char dataBuffer[1024];
    acceptSocket.receiveData(dataBuffer, 1024);

    cout << "Received data: " << dataBuffer << endl;

    acceptSocket.sendData(acceptSocket.handshakeIn(dataBuffer, OwnVersion)); // Handle Handshake

    // 6. recieve data

    string connectResponse(dataBuffer, 14);
    if (connectResponse == "INFO2 CONNECT/")
    {

        memset(dataBuffer, 0, 1024);

        acceptSocket.receiveData(dataBuffer, 1024);

        string connectResponse(dataBuffer);
        // cout << "dataBuffer string " << connectResponse << endl;

        if (sizeof(dataBuffer) > 0)
        {

            string BackconnectResponse(dataBuffer, 11);
            string FriendRqResponse(dataBuffer, 18);
            string SENDResponse(dataBuffer, 4);

            if (!(strcmp(BackconnectResponse.c_str(), "BACKCONNECT")))
            {
                cout << "receved backConnection attempt" << endl;
                string responseIP(dataBuffer + 12);
                cout << responseIP;
                if (checkIP(knownClients, responseIP))
                {
                    acceptSocket.closeSocket();
                    firstHandshake(responseIP, PORT, OwnVersion, knownClients);
                }
                else
                    cout << responseIP << " is already a known client" << endl;
            }
            else if (!(strcmp(BackconnectResponse.c_str(), "FRIEND REQUEST\n\n")))
            {
                string IP;
                IP = giveIP(knownClients);
                acceptSocket.sendData(IP);
                acceptSocket.closeSocket();
            }
            else if (!(strcmp(SENDResponse.c_str(), "SEND")))
            {
                string MessageToForward(dataBuffer);
                string RecevedMessageID(dataBuffer + 5, 11);
                stringstream ss3;
                ss3 << RecevedMessageID;
                int RecevedMessageIDint;
                ss3 >> RecevedMessageIDint;

                if (!checkMessageID(MessageIDs, RecevedMessageIDint)) // Check if messageID is already known
                {
                    storeMessageID(MessageIDs, RecevedMessageIDint);                                             // Adds messageID to already known messagIDs
                    sendMessageToClients(MessageToForward, RecevedMessageIDint, knownClients, PORT, OwnVersion); // forwars message to all clients
                }
                acceptSocket.closeSocket();
            }
            else
            {

                cout << "Request unknown: " << connectResponse << endl;
                acceptSocket.closeSocket();
            }
        }
    } // Ende der Schleife

    //cout << "Thread closed" << endl;
    return threadNum;
}

int listenThreading(string ownIP, double OwnVersion, vector<string> &knownClients, vector<int> &MessageIDs, int threadNum)
{

again:
    if (listenHandler(ownIP, OwnVersion, knownClients, MessageIDs, 1))
    {
    }
    else
    {
        return 0;
    }

    goto again;
}