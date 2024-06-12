#include "h2.h"

//#include "initWinsock.h"
#include "Header2.h"


#define PORT 26000

using namespace std;

bool FirstTimeconnect(string firstIP, int clientsocket) {
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    char buffer[12];


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

    // P2PInfo2-Handshake
    string handshakeMessage = "INFO2 CONNECT/0.6\n\n";
    send(clientsocket, handshakeMessage.c_str(), handshakeMessage.length(), 0);

    
    // ... (Antwort vom Server empfangen und überprüfen)

    listen(clientsocket, 5);
    recv(clientsocket, buffer, sizeof(buffer), 0);
    cout << "Server antwortet: " << buffer << endl;

    if(buffer[] == "INFO2 OK\n\n"){
        return true;
    }
    else{
        return false;
    
    }


    return true;
}