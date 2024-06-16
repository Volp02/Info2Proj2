#ifdef _WIN32 // Windows-spezifischer Code
#include "initWinsock.h"
typedef int socklen_t;
#else // Linux-spezifischer Code
#include "linuxLib.h"
#define closesocket close
#endif

#include <iostream>
#include <string.h>
#include <string>
#include <thread>
#include <vector>
#include <sstream>

#include "connect.h"
#include "listen.h"
#include "DataStorage.h"
#include "socket.h"

#define PORT 26000
#define BUFFER_SIZE 1024

using namespace std;

int main()
{

#ifdef _WIN32
    initWinsock();
#endif

    cout << "enter own IP (or only last 3 digits):" << endl;

    string addressStart = "192.168.178.";
    string own_address;
    std::vector<SocketClss> establishedConnections;
    std::vector<int> usedMsgIDs;

    getline(cin, own_address);

    if (size(own_address) <= 3)
    {
        own_address = addressStart + own_address;
        cout << "Address set to " << own_address << endl;
    }

    cout << "enter initial Servent IP (or only last 3 digits):" << endl;

    string initServerIP;

    getline(cin, initServerIP);

    if (size(initServerIP) <= 3)
    {
        initServerIP = addressStart + initServerIP;
        cout << "Address set to " << initServerIP << endl;
    }

    bool firstUsr;
    cout << "are you the first node? (1 / 0)";
    cin >> firstUsr;

    double version = 0.6;

    // listenForIncomingConnection, own_address, version, knownIPs, usedMsgIDs;
    SocketClss ServerSocket;

    // listenForIncomingConnection(std::ref(ServerSocket), own_address, version, std::ref(establishedConnections),std::ref(usedMsgIDs));

    // thread listening(listenForIncomingConnection(std::ref(ServerSocket), own_address, version, std::ref(establishedConnections),std::ref(usedMsgIDs)));
    ServerSocket.S_createAndBind(PORT); // create and bind socket
    ServerSocket.S_listen();            // listen for incoming connections


    std::thread listening([&]()
                          { listenForIncomingConnection(std::ref(ServerSocket), own_address, version,
                                                        std::ref(establishedConnections), std::ref(usedMsgIDs)); });


    if (!firstUsr)
    {

        SocketClss FirstConnectSocket;
        cout << "connecting to server :" << initServerIP << endl;
        FirstConnectSocket.C_createAndConnect(initServerIP, PORT); // connect to server

        FirstConnectSocket.sendData("INFO2 CONNECT/" + std::to_string(version)); // send handshake

        char dataBuffer[1024] = {0};
        int recievedData = FirstConnectSocket.receiveData(dataBuffer, 1024); // receive handshake

        string connectResponse(dataBuffer);

        if (recievedData >= 0 && connectResponse == "INFO2 OK\n\n")
        { // check handshake response
            cout << "handshake successful\n"
                 << endl;
            storeIP(establishedConnections, FirstConnectSocket);       // store server IP
            cout<<
            FirstConnectSocket.sendData("BACKCONNECT " + own_address); // send backconnect
            FirstConnectSocket.closeSocket();                          // close socket
        }
        else
        {
            cout << "handshake not successful, returning" << "connectResponse == INFO2 OK: " << (connectResponse == "INFO2 OK\n\n") << endl; // handshake not successful, close socket
            FirstConnectSocket.closeSocket();
        }
    }

    

    /*
    SocketClss InitSocket;
    InitSocket = firstHandshake(own_address, PORT, version);

    if (!firstUsr) {
        if (InitSocket.sockfd >= 0) {

            string knownClient = initServerIP;
            cout << "connected to client " << knownClient << endl;

            InitSocket.sendData("BACKCONNECT " + own_address);

            InitSocket.sendData("FRIEND REQUEST\n\n");
            char dataBuffer[128] = { 0 };

            InitSocket.receiveData(dataBuffer, 128);

            string IPasString(dataBuffer);
            storeIP(knownIPs, IPasString);

        }
        else {
            cout << "no connection established, acting as first Node!" << endl;
            storeIP(knownIPs, own_address);
        }
    }*/

    listening.join();

    return 0;
}