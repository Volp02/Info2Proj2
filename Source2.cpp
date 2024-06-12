#include "h2.h"

//#include "initWinsock.h"
#include "Header2.h"


#define PORT 26000

using namespace std;

bool FirstTimeconnect(string firstIP, int clientsocket) {
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    char buffer[16];


    // IP-Adresse konvertieren
    if (inet_pton(AF_INET, firstIP.c_str(), &serverAddr.sin_addr) <= 0) {
        cerr << "Ungültige IP-Adresse: " << firstIP << endl;
        return false;
    }

    // Verbindung herstellen
    if (connect(clientsocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Verbindungsfehler: " << strerror(errno) << endl;
        return false;
    }

    // P2PInfo2 handshake
    string handshakeMessage = "INFO2 CONNECT/0.6\n\n";
    send(clientsocket, handshakeMessage.c_str(), handshakeMessage.length(), 0);

    // Receive server response
    int valread = read(clientsocket, buffer, 16);
    cout << "Server response: " << buffer << endl;

    // Check if handshake was successful
    if (strcmp(buffer, "INFO2 OK\n\n") == 0) {
        return true;
    } else {
        return false;
    }


    return false;
}