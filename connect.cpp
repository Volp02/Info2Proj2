#include "connect.h"
#include <sstream>

#include "initWinsock.h"

//#include "linuxLib.h"

#define PORT 26000

using namespace std;

bool FirstTimeconnect(string firstIP, float version) {
    
    struct sockaddr_in serv_addr;  // Struktur für die Server-Adresse
    int client_socket;             // Socket-Descriptor des Clients

    //creat socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0) {  
        std::cout << "client socket setup failed" << std::endl;
        return 1;
    }


    // 2. Server-Adresse konfigurieren
    serv_addr.sin_family = AF_INET;             // IPv4-Adressfamilie
    serv_addr.sin_port = htons(PORT);           // Portnummer in Netzwerk-Byte-Reihenfolge umwandeln

    // Konvertiert IPv4-Adresse von Text zu Binärformat
    if (inet_pton(AF_INET, firstIP.c_str(), &serv_addr.sin_addr) <= 0) { 
        std::cerr << "Ungültige Adresse oder Adresse nicht unterstützt\n";
        return 1; // Beenden mit Fehlercode 1
    }

    std::cout << "Adresse Konvertiert\n";

    // 3. Verbindung zum Server herstellen
    if (connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Verbindungsfehler: " << strerror(errno) << std::endl;
        std::cerr << "Verbindungsfehler\n";
        return 1; // Beenden mit Fehlercode 1
    }
    std::cout << "Verbunden zum Server\n";
    
    // 4. Nachricht senden
    std::stringstream ss;
    string Sversion;
    ss << version;
    ss >> Sversion;
  
    string message = "INFO2 CONNECT/" + Sversion + "\n\n";

    send(client_socket, message.c_str(), message.length(), 0); // Senden der Nachricht an den Server
    std::cout << "Nachricht gesendet: " << message << std::endl;

    // 5. Antwort empfangen
    char buffer[16] ={0};
    
    recv(client_socket,buffer,sizeof(buffer),0);

    string response(buffer,3);
    close(client_socket);

    if(!strcmp(response.c_str(), "OK/")){
        std::cout << "handshake failed\n";
        return false;
    }

    string responseVers(buffer + 6);
        ss << responseVers;
        float ServerVersion;
        ss >> ServerVersion;
        if(ServerVersion >= version){
            std::cout << "handshake successful\n";
            return true;
        }
        else{
            
        }

    // 5. Verbindung schließen
    close(client_socket);

    return 0; // Erfolgreiche Beendigung

}
