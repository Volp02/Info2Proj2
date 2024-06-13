#include "connect.h"
#include <sstream>

#include "initWinsock.h"
//#include "linuxLib.h"
//close  -  closesocket

#define PORT 26000

using namespace std;

bool FirstTimeconnect(string firstIP, float version)
{

    struct sockaddr_in serv_addr; // Struktur für die Server-Adresse
    int client_socket;            // Socket-Descriptor des Clients

    // creat socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0)
    {
        std::cout << "client socket setup failed" << std::endl;
        return false;
    }

    // 2. Server-Adresse konfigurieren
    serv_addr.sin_family = AF_INET;   // IPv4-Adressfamilie
    serv_addr.sin_port = htons(PORT); // Portnummer in Netzwerk-Byte-Reihenfolge umwandeln

    // Konvertiert IPv4-Adresse von Text zu Binärformat
    if (inet_pton(AF_INET, firstIP.c_str(), &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "Ungültige Adresse oder Adresse nicht unterstützt\n";
        return false; // Beenden mit Fehlercode 1
    }

    std::cout << "Adresse Konvertiert\n";

    // 3. Verbindung zum Server herstellen
    if (connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        //std::cerr << "Verbindungsfehler: " << strerror(errno) << std::endl;
        std::cerr << "Verbindungsfehler\n";
        return false; // Beenden mit Fehlercode 1
    }
    std::cout << "Verbunden zum Server\n";

    // 4. initiate handshake
    std::stringstream ss;
    string Sversion;
    ss << version;
    ss >> Sversion;

    string message = "INFO2 CONNECT/" + Sversion + "\n\n";

    send(client_socket, message.c_str(), message.length(), 0); // Senden der Nachricht an den Server
    std::cout << "Nachricht gesendet: " << message << std::endl;

    // 5. Antwort empfangen
    char buffer[16] = {0};

    recv(client_socket, buffer, sizeof(buffer), 0);

    string response(buffer);
    closesocket(client_socket);

    if (!strcmp(response.c_str(), "INFO2 OK\n\n"))
    {
        std::cout << "success handshake\n";
        std::cout << response.c_str() << endl;
        return true;
    }

    cout << "failed handshake\n";
    cout << response.c_str() << endl;
    return false;

    // 5. Verbindung schließen
    closesocket(client_socket);

    return false; // Erfolgreiche Beendigung
}
