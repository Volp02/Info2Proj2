//----- winsock -------
//#include "initWinsock.h"
//---------------------


//----- linux lib -----
#include "Header2.h"
//---------------------

#include <iostream>
#include <string.h>

#include "h2.h"


#define PORT 26000
#define BUFFER_SIZE 1024


using namespace std;

int main() {

    // -------- init winsock --------
    //initWinsock();
    //-------------------------------

    cout << "hello wolrd!" << endl;

    bool firstUsr = true;
    string version = "0.1";
    char buffer[BUFFER_SIZE] = { 0 }; // Buffer für Empfangene Daten

    cout << "Input own address:" << endl;
    string own_address;
    cin >> own_address;

    if (size(own_address) <= 7) {
        own_address = "127.0.0.1";
        cout << "loopback: Address set to 127.0.0.1" << endl;
    }

    int client_socket;      //variable für Socket-Deskriptoren
    int MAX_PENDING_CONNECTIONS = 5; // Maximale Anzahl wartender Verbindungen

    struct sockaddr_in client_address;          // client Socket-Adresse

    client_address.sin_family = AF_INET;            // IPv4
    client_address.sin_addr.s_addr = INADDR_ANY;    // Any IP
    client_address.sin_port = htons(PORT);          // Port setzen

    int addrlen = sizeof(struct sockaddr_in); // Adresse des Clients

    // IP-Adresse konvertieren
    if (inet_pton(AF_INET, own_address.c_str(), &client_address.sin_addr) <= 0) {
        cerr << "Ungültige IP-Adresse: " << own_address << endl;
        return false;
    }

    // 2. Sockets erstellen
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {   // IPv4 und TCP-Verfahren
        std::cout << "client socket setup failed" << std::endl;
        return 1;
    }

    // 3. Sockets an  Port binden
    if (bind(client_socket, (struct sockaddr*)&client_address, sizeof(client_address)) < 0) {    // IPv4 und TCP-Verfahren
        std::cout << "client bind failed" << std::endl;
        return 1;
    }

    FirstTimeconnect("127.0.0.1",client_socket);
}