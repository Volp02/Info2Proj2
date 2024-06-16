#ifdef _WIN32 // Windows-spezifischer Code
#include "initWinsock.h"
typedef int socklen_t;
#else // Linux-spezifischer Code
#include "linuxLib.h"
#define closesocket close
#endif


#include "connect.h"
#include <sstream>
#include "socket.h"

#define PORT 26000

using namespace std;


SocketClss firstHandshake(string IP, int Port, double OwnVersion) {

    SocketClss serverSocket; // Erstelle ein MySocket-Objekt 
    if (!serverSocket.C_createAndConnect(IP, Port)) {
        std::cerr << "Fehler beim Erstellen oder Binden des Sockets!" << std::endl;
        return SocketClss(); // Rückgabe eines leeren MySocket-Objekts im Fehlerfall
    }

    serverSocket.sendData("INFO2 CONNECT/" + std::to_string(OwnVersion));

    char dataBuffer[1024] = { 0 };
    int receiveData = serverSocket.receiveData(dataBuffer, 1024);

    if (receiveData <= 0) {
        std::cerr << "Fehler beim Empfangen von Daten." << std::endl;
        serverSocket.closeSocket();
        return SocketClss();
    }

    std::string connectResponse(dataBuffer, 14);
    if (connectResponse == "INFO2 CONNECT/") {

        std::cout << "received Connection attempt" << std::endl;
        std::stringstream ss2;
        std::string responseVers(dataBuffer + 14);
        ss2 << responseVers;
        double clientVersion;
        ss2 >> clientVersion;

        if (clientVersion >= OwnVersion) {
            std::cout << "handshake successful\n";
            std::string acceptConnection = "INFO2 OK\n\n";

            serverSocket.sendData(acceptConnection); // Verwende sendData von MySocket
            std::cout << "responds with INFO2 OK! " << std::endl;

            serverSocket.closeSocket(); // Schließe den Server-Socket nach dem Handshake
            return serverSocket; // Gib den Client-Socket zurück
        }
        else {
            std::cout << "handshake failed -> old version: " << clientVersion << endl;
        }
        std::cout << "handshake failed\n";
    }

    serverSocket.closeSocket(); // Schließe den Server-Socket nach dem Handshake
    return SocketClss(); //Gib den leere socket class zurück
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
bool sendMessage(std::string Message, int MessageID, std::vector<std::string> &knownClients) {

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

bool sendMessageIDlessDEBUG(std::string Message, std::vector<std::string> &knownClients){

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
            std::cerr << "sendMessageDEBUG: Ungültige Adresse oder Adresse nicht unterstützt\n";
            return false; // Beenden mit Fehlercode 1
        }
        // 3. Verbindung zum Server herstellen
        if (connect(clientIP_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        {
            //std::cerr << "Verbindungsfehler: " << strerror(errno) << std::endl;
            std::cerr << "sendMessageDEBUG: Verbindungsfehler\n Couldnt connect to Server " << knownClients[i] << endl;
            return false; // Beenden mit Fehlercode 1
        }
        std::cout << "sendMessageDEBUG: Verbunden zum Server " << knownClients[i] << endl;


        string message = Message;

        send(clientIP_socket, message.c_str(), message.length(), 0); // Senden der Nachricht an den Server
        std::cout << "sendMessage: Nachricht gesendet: " << message << std::endl;

        char buffer[128] = { 0 };

        recv(clientIP_socket, buffer, sizeof(buffer), 0);

        string response(buffer);

        cout << "receved answer: " << response;
        closesocket(clientIP_socket);

    }
    return true;
}
