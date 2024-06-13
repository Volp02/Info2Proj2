//----- winsock -------
#include "initWinsock.h"
//---------------------


//----- linux lib -----
//#include "linuxLib.h"
//---------------------

#include <iostream>
#include <string.h>
#include <string>

#include "connect.h"
#include "listen.h"


#define PORT 26000
#define BUFFER_SIZE 1024


using namespace std;

int main() {

    // -------- init winsock --------
    initWinsock();
    //-------------------------------

    cout << "enter own IP:" << endl;
    
    string own_address;

    getline(cin, own_address);

    if (size(own_address) <= 7) {
        own_address = "192.168.178.163";
        cout << "loopback: Address set to 192.168.178.163" << endl;
    }

    cout << "enter initial Servent IP:" << endl;

    string initServer;

    getline(cin, initServer);

    if (size(initServer) <= 7) {
        initServer = "192.168.178.163";
        cout << "loopback: Address set to 192.168.178.163" << endl;
    }

    bool firstUsr = true;
    double version = 0.7;

    if (firstUsr) {
        if (FirstTimeconnect(initServer, version)) {
            string knownClient = initServer;
            cout << "connected to client " << knownClient << endl;
        }
        else {
            cout << "no connection established, acting as first Node!" << endl;
        }
    }
    listenForIncomingConnection(own_address);
    //FirstTimeconnect("192.168.178.163", version);

}