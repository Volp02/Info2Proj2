//----- winsock -------
#include "initWinsock.h"
//---------------------


//----- linux lib -----
//#include "linuxLib.h"
//---------------------

#include <iostream>
#include <string.h>
#include <string>
#include <thread>

#include "connect.h"
#include "listen.h"


#define PORT 26000
#define BUFFER_SIZE 1024


using namespace std;

int main() {

    // -------- init winsock --------
    //initWinsock();
    //-------------------------------

    cout << "enter own IP (or only last 3 digits):" << endl;
    
    string addressStart = "192.168.178.";
    string own_address;

    getline(cin, own_address);
    
    if (sizeof(own_address) <= 3) {
        own_address = addressStart + own_address;
        cout << "Address set to " << own_address << endl;
    }

    cout << "enter initial Servent IP (or only last 3 digits):" << endl;

    string initServer;

    getline(cin, initServer);

    if (sizeof(initServer) <= 3) {
        initServer = addressStart + initServer;
        cout << "Address set to " << initServer << endl;
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

    thread t1(listenForIncomingConnection, own_address); // thread #2
    thread t2(FirstTimeconnect, initServer, version); // thread #3


    t1.join(); 
    t2.join(); 
    return 0;

    //listenForIncomingConnection(own_address);
    //FirstTimeconnect("192.168.178.163", version); 

}