/*
Projektarbeit 2 – P2PInfo
Gruppe:
Andreas Runge
Matthias Friedl
Lajos Billes

Wir stimmen der Veröffentlichung unseres Source Code in anonymisierter Form zu.

Copyright (C) 2024 [authors #1-#4]
SPDX-License-Identifier: MIT
*/

#ifdef _WIN32 // Windows-spezifischer Code
#include "initWinsock.h"
#include <windows.h>
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

    std::string addressStart;
    addressStart = "192.168.178.";

    std::string own_address;
    std::vector<string> knownClients;
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

    char input;

    cout << "First user? (1/0) " << endl;
    cin >> firstUsr;

    double version = 0.6;

    //SocketClss ListeningSocket;
    thread t1(listenThreading, own_address, version, std::ref(knownClients), std::ref(usedMsgIDs), 1);



    if (!firstUsr) {

        SocketClss ConnectSocket;

        cout << "connecting to " << initServerIP << endl;
        ConnectSocket.C_createAndConnect(initServerIP, PORT); // connect to first known server
        cout << "connected to " << initServerIP << endl;
        if (ConnectSocket.handshakeOut(version)) {
            cout << "handshake successful" << endl;
            storeIP(knownClients, initServerIP);
            ConnectSocket.sendData("BACKCONNECT " + own_address);
            ConnectSocket.closeSocket();

        }
        else {
            cout << "handshake not successful, returning" << endl;
            ConnectSocket.closeSocket();
            return 0;
        }

    }

    string messageInput;
    bool endProgram = true;
    while (!endProgram) {

        cout << "message to send: ";
        cin >> messageInput;
        (cin, messageInput);

        if (messageInput == "q") {
            endProgram = true;

        }
        else {
            sendMessageToClients("Tets message!", createMessageID(usedMsgIDs), knownClients, PORT, version);
        }

        //thread t2(listenHandler, own_address, version, std::ref(knownClients), std::ref(usedMsgIDs)); 

        t1.join();
        //t2.join();


        return 0;
    }

}