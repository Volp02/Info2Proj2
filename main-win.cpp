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

#include "connect.h"
#include "listen.h"
#include "DataStorage.h"
#include "socket.h"


#define PORT 26000
#define BUFFER_SIZE 1024

using namespace std;


static int waitForConnection(double version, string own_address, vector<string>& knownIPs, vector<int>& usedMsgIDs) {

    SocketClss InitSocketIncoming;
    while (true) {
        InitSocketIncoming = HandleFirstHandshake(PORT, version);
        if (InitSocketIncoming.sockfd >= 0) {
            thread* lissteningThread = new thread([&]() {
                listenForIncomingConnection(std::ref(InitSocketIncoming), own_address, version, std::ref(knownIPs), std::ref(usedMsgIDs));
                });
        }
    }

}

int main() {

    #ifdef _WIN32 
        initWinsock();
    #endif

    cout << "enter own IP (or only last 3 digits):" << endl;
    
    string addressStart = "192.168.178.";
    string own_address;
    std::vector<std::string> knownIPs;
    std::vector<int> usedMsgIDs;

    getline(cin, own_address);
    
    if (size(own_address) <= 3) {
        own_address = addressStart + own_address;
        cout << "Address set to " << own_address << endl;
    }

    cout << "enter initial Servent IP (or only last 3 digits):" << endl;

    string initServerIP;

    getline(cin, initServerIP);

    if (size(initServerIP) <= 3) {
        initServerIP = addressStart + initServerIP;
        cout << "Address set to " << initServerIP << endl;
    }
    
    bool firstUsr;
    cout << "are you the first node? (1 / 0)";
    cin >> firstUsr;
    
    double version = 0.6;


    //listenForIncomingConnection, own_address, version, knownIPs, usedMsgIDs;

    thread t1(waitForConnection,version,own_address, std::ref(knownIPs), std::ref(usedMsgIDs));
    
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
    }
    

    t1.join(); 
 
    return 0;
}