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
    cout << "FirstTimeConnect: connectint to:  " << firstIP << endl;
    struct sockaddr_in serv_addr; // Struktur für die Server-Adresse
    int client_socket;            // Socket-Descriptor des Clients

    // creat socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0)
    {
        std::cout << "client socket setup failed" << std::endl;
        return false;
    }

    cout <<"FirstTimeConnect: socket created\n";

    // 2. Server-Adresse konfigurieren
    serv_addr.sin_family = AF_INET;   // IPv4-Adressfamilie
    serv_addr.sin_port = htons(PORT); // Portnummer in Netzwerk-Byte-Reihenfolge umwandeln

    cout << "FirstTimeConnect: port set\n";  

    // Konvertiert IPv4-Adresse von Text zu Binärformat
    if (inet_pton(AF_INET, firstIP.c_str(), &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "Ungültige Adresse oder Adresse nicht unterstützt\n";
        return false; // Beenden mit Fehlercode 1
    }

    //std::cout << "Adresse Konvertiert\n";

    // 3. Verbindung zum Server herstellen
    connect(client_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    #ifdef _WIN32
        int error_code = WSAGetLastError(); // Fehlercode abrufen
        std::cerr << "Verbindungsfehler: " << error_code << std::endl;
    #else
        std::cerr << "Verbindungsstatus: " << strerror(errno) << std::endl;
    #endif
    
    std::cout << "FirstTimeConnect: Verbunden zum Server " << firstIP << endl ;

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
        std::cout << "FirstTimeConnect: handshake succsessfull -> receved 'INF2 OK'!\n";
        std::cout << response.c_str() << endl;
        return true;
    }

    cout << "FirstTimeConnect: handshake failed\nNo connection established with " << firstIP;
    //cout << response.c_str() << endl;

    // 5. Verbindung schließen
    closesocket(client_socket);

    return false; // no connection
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
        std::cerr << "Backconnect: Ungültige Adresse oder Adresse nicht unterstützt bei backconnect\n";
        return false; // Beenden mit Fehlercode 1
    }
    // 3. Verbindung zum Server herstellen
    if (connect(clientIP_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        //std::cerr << "Verbindungsfehler: " << strerror(errno) << std::endl;
        std::cerr << "Backconnect: Verbindungsfehler\n Couldnt connect to Server" << sendIP << endl;
        return false; // Beenden mit Fehlercode 1
    }
    std::cout << "Backconnect: Verbunden zum Server " << sendIP << endl;


    string message = "BACKCONNECT " + ownIP;

    send(clientIP_socket, message.c_str(), message.length(), 0); // Senden der Nachricht an den Server
    std::cout << "Backconnect: Nachricht gesendet: " << message << std::endl;
    return true;
}
string sendFriendRequest(std::string targetIP) {

    struct sockaddr_in serv_addr; // Struktur für die Server-Adresse
    int clientIP_socket;            // Socket-Descriptor des Clients

    // creat socket
    if ((clientIP_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0)
    {
        std::cout << "client socket setup failed" << std::endl;
        return "error";
    }
    serv_addr.sin_family = AF_INET;   // IPv4-Adressfamilie
    serv_addr.sin_port = htons(PORT); // Portnummer in Netzwerk-Byte-Reihenfolge umwandeln

    // Konvertiert IPv4-Adresse von Text zu Binärformat
    if (inet_pton(AF_INET, targetIP.c_str(), &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "sendFriendRequest: Ungültige Adresse oder Adresse nicht unterstützt\n";
        return "error"; // Beenden mit Fehlercode 1
    } 
    // 3. Verbindung zum Server herstellen
    if (connect(clientIP_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        //std::cerr << "Verbindungsfehler: " << strerror(errno) << std::endl;
        std::cerr << "sendFriendRequest: Verbindungsfehler\n Couldnt connect to Server" << targetIP << endl;
        return "error"; // Beenden mit Fehlercode 1
    }
    std::cout << "sendFriendRequest: Verbunden zum Server " << targetIP << endl;


    string payload = "FRIEND REQUEST\n\n";

    send(clientIP_socket, payload.c_str(), payload.length(), 0); // Senden der Nachricht an den Server
    std::cout << "sendFriendRequest: Nachricht gesendet: " << payload << std::endl;

    char buffer[12] = { 0 };

    recv(clientIP_socket, buffer, sizeof(buffer), 0);

    string response(buffer);

    closesocket(clientIP_socket);

    return response;
    
    // 5. Verbindung schließen
    
}
bool sendMessage(std::string Message, int MessageID, std::vector<std::string> knownClients) {

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
    for (int i = 0; i <= sizeof(knownClients); i++) {

        if (inet_pton(AF_INET, knownClients[i].c_str(), &serv_addr.sin_addr) <= 0)
        {
            std::cerr << "sendMessage: Ungültige Adresse oder Adresse nicht unterstützt\n";
            return false; // Beenden mit Fehlercode 1
        }
        // 3. Verbindung zum Server herstellen
        if (connect(clientIP_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        {
            //std::cerr << "Verbindungsfehler: " << strerror(errno) << std::endl;
            std::cerr << "sendMessage: Verbindungsfehler\n Couldnt connect to Server " << knownClients[i] << endl;
            return false; // Beenden mit Fehlercode 1
        }
        std::cout << "sendMessage: Verbunden zum Server " << knownClients[i] << endl;


        string message = "SEND  " + MessageID + ' ' + Message;

        send(clientIP_socket, message.c_str(), message.length(), 0); // Senden der Nachricht an den Server
        std::cout << "sendMessage: Nachricht gesendet: " << message << std::endl;

    }
    return true;
    
    


}
