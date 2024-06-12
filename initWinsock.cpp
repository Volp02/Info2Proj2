#include <winsock2.h>
#include <ws2tcpip.h>
#include "initWinsock.h"
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")
using namespace std;

int initWinsock() {

    WORD wVersionRequest;						//typedef für unsigned short = 16bit = 2byte
    WSADATA wsaData;							//erschaffe Objekt des structs "WSDATA" (s. OneNote)
    wVersionRequest = MAKEWORD(2, 2);			//V.2.2 der Winsock-API

    int err = WSAStartup(wVersionRequest, &wsaData);	//gibt 0 zurück wenn alles gut läuft

    if (err != 0)
    {
        cout << "WSAStartup failed with error: " << err << endl;
        WSACleanup();
        return err;
    }
    else
    {
        cout << "WSA Startup successful!" << endl;


    }

}
    