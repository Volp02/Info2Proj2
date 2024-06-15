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

#include "connect.h"
#include "listen.h"


#define PORT 26000
#define BUFFER_SIZE 1024


using namespace std;

int main() {

    #ifdef _WIN32 
        initWinsock();
    #endif
    
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
    float version = 0.7;


    thread t1(listenForIncomingConnection, own_address, version); // thread #2


    if (firstUsr) {
        if (FirstTimeconnect(initServer, version)) {

            string knownClient = initServer;
            cout << "connected to client " << knownClient << endl;
        }
        else {
            cout << "no connection established, acting as first Node!" << endl;
        }
    }


    t1.join(); 
 
    return 0;

    //listenForIncomingConnection(own_address);
    //FirstTimeconnect("192.168.178.163", version); 

}