//----- winsock -------
#include "initWinsock.h"
//---------------------


//----- linux lib -----
//#include "linuxLib.h"
//---------------------

#include <iostream>
#include <string.h>

#include "connect.h"


#define PORT 26000
#define BUFFER_SIZE 1024


using namespace std;

int main() {

    // -------- init winsock --------
    initWinsock();
    //-------------------------------

    cout << "hello wolrd!" << endl;

    bool firstUsr = true;
    float version = 0.7;
    char buffer[BUFFER_SIZE] = { 0 }; // Buffer für Empfangene Daten

    cout << "Input own address:" << endl;
    string own_address;
    cin >> own_address;

    if (size(own_address) <= 7) {
        own_address = "127.0.0.1";
        cout << "loopback: Address set to 127.0.0.1" << endl;
    }

    
    //listen(client_socket, 3); // Auf Verbindungen warten

    FirstTimeconnect("192.168.178.25", version);



}