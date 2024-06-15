#ifdef _WIN32 // Windows-spezifischer Code
#include "initWinsock.h"
typedef int socklen_t;
#else // Linux-spezifischer Code
#include "linuxLib.h"
#define closesocket close
#endif


#include "connect.h"
#include <sstream>

#define PORT 26000

using namespace std;

bool FirstTimeconnect(string firstIP, float version)
{
    cout << "FirstTimeConnect " << firstIP << endl;
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

    //std::cout << "Adresse Konvertiert\n";

    // 3. Verbindung zum Server herstellen
    if (connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        //std::cerr << "Verbindungsfehler: " << strerror(errno) << std::endl;
        std::cerr << "Verbindungsfehler\n Couldnt connect to Server" << firstIP << endl;
        return false; // Beenden mit Fehlercode 1
    }
    std::cout << "Verbunden zum Server " << firstIP << endl ;

    // 4. initiate handshake
    std::stringstream ss;
    string Sversion;
    ss << version;
    ss >> Sversion;

    string message = "INFO2 CONNECT/" + Sversion + "\n\n";

    send(client_socket, message.c_str(), message.length(), 0); // Senden der Nachricht an den Server
    //std::cout << "Nachricht gesendet: " << message << std::endl;

    // 5. Antwort empfangen
    char buffer[16] = {0};

    recv(client_socket, buffer, sizeof(buffer), 0);

    string response(buffer);
    closesocket(client_socket);

    if (!strcmp(response.c_str(), "INFO2 OK\n\n"))
    {
        std::cout << "handshake succsessfull \n";
        std::cout << response.c_str() << endl;
        return true;
    }

    cout << "handshake failed\nNo connection established with " << firstIP;
    //cout << response.c_str() << endl;
    return false;

    // 5. Verbindung schließen
    closesocket(client_socket);

    return false; // Erfolgreiche Beendigung
}
bool backConnectSend(string ownIP, string sendIP)
{
    struct sockaddr_in serv_addr; // Struktur für die Server-Adresse
    int clientIP_socket;            // Socket-Descriptor des Clients

    // creat socket
    if ((clientIP_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0)
    {
        std::cout << "client socket setup failed" << std::endl;
        return false;
    }
    serv_addr.sin_family = AF_INET;   // IPv4-Adressfamilie
    serv_addr.sin_port = htons(PORT); // Portnummer in Netzwerk-Byte-Reihenfolge umwandeln

    // Konvertiert IPv4-Adresse von Text zu Binärformat
    if (inet_pton(AF_INET, sendIP.c_str(), &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "Ungültige Adresse oder Adresse nicht unterstützt bei backconnect\n";
        return false; // Beenden mit Fehlercode 1
    }
    // 3. Verbindung zum Server herstellen
    if (connect(clientIP_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        //std::cerr << "Verbindungsfehler: " << strerror(errno) << std::endl;
        std::cerr << "Verbindungsfehler\n Couldnt connect to Server" << sendIP << endl;
        return false; // Beenden mit Fehlercode 1
    }
    std::cout << "Verbunden zum Server " << sendIP << endl;


    string message = "BACKCONNECT " + ownIP;

    send(clientIP_socket, message.c_str(), message.length(), 0); // Senden der Nachricht an den Server
    //std::cout << "Nachricht gesendet: " << message << std::endl;
    return true;
}