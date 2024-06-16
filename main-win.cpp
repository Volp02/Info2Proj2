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

static int listenForMessage(SocketClss socket, double version, string own_address, vector<SocketClss>& establishedConnections, vector<int>& usedMsgIDs) {
 
    cout << "listenForMessage started a process" << endl;
    
    listenForIncomingConnection(socket, own_address, version, establishedConnections, usedMsgIDs);
    
    return 0;
    
}




int restOfProgramm(bool firstUsr, string initServerIP, vector<SocketClss>& establishedConnections){
    if (!firstUsr)
    {
        cout << "restOfProgramm started a process" << endl;

        SocketClss FirstConnectSocket;
        cout << "connecting to server :" << initServerIP << endl;
        FirstConnectSocket.C_createAndConnect(initServerIP, PORT); // connect to server

        FirstConnectSocket.sendData("INFO2 CONNECT/0.6\n\n"); // send handshake

        char dataBuffer[1024] = {0};
        cout << "recieving handshake response"<< endl;
        int recievedData = FirstConnectSocket.receiveData(dataBuffer, 1024); // receive handshake

        string connectResponse(dataBuffer);

        if (recievedData >= 0 && connectResponse == "INFO2 OK\n\n")
        { // check handshake response
            cout << "handshake successful\n"
                 << endl;
            storeIP(establishedConnections, FirstConnectSocket);       // store server IP
            
        }
        else
        {
            cout << "handshake not successful, returning" << "connectResponse == INFO2 OK: " << (connectResponse == "INFO2 OK\n\n") << endl; // handshake not successful, close socket
            FirstConnectSocket.closeSocket();
        }
    }
    return 0;
}


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


    restOfProgramm(firstUsr, initServerIP, establishedConnections);
    
    SocketClss InitSocket;


    storeIP(establishedConnections, firstHandshakeHandler(own_address, version, establishedConnections, usedMsgIDs));
    
    cout << "Handled firsttimeconnect" << endl;
    
    listenForIncomingConnection(establishedConnections[0],own_address, version, establishedConnections, usedMsgIDs);

    


















    //InitSocket = firstHandshakeHandler(own_address, version, establishedConnections, usedMsgIDs);
    
    //thread t1(ListenForConnections, own_address, version, std::ref(establishedConnections), std::ref(usedMsgIDs));
    //thread t3(restOfProgramm,firstUsr, initServerIP, std::ref(establishedConnections));

    //ListenForConnections(own_address, version, std::ref(establishedConnections), std::ref(usedMsgIDs));
    //restOfProgramm(firstUsr, initServerIP, std::ref(establishedConnections));


    //t2.join()

    

    //t1.join();
    
    //t3.join();



    return 0;
}