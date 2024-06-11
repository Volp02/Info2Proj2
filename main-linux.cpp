#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>  // für inet_pton
#include <unistd.h>     // für close
#include <string.h>


#define PORT 26000
#define BUFFER_SIZE 1024


using namespace std;

int main() {

	cout << "hello wolrd!" << endl;
	
    bool firstUsr = true;
    string version = "0.1";    
    
    cout << "Input own address:" << endl;
    string own_address;
    cin >> own_address;
    
    if(size(own_address) <= 7){
        own_address = "127.0.0.1";
        cout << "loopback: Address set to 127.0.0.1" <<endl;
    }


    int server_socket, client_socket, new_socket;      //variable für Socket-Deskriptoren
    int MAX_PENDING_CONNECTIONS = 5; // Maximale Anzahl wartender Verbindungen
    
    struct sockaddr_in server_address;         // server Socket-Adresse
    struct sockaddr_in client_address;          // client Socket-Adresse

    server_address.sin_family = AF_INET;        // IPv4
    server_address.sin_addr.s_addr = INADDR_ANY; // Any IP
    server_address.sin_port = htons(PORT);       // Port setzen

    client_address.sin_family = AF_INET;        // IPv4
    client_address.sin_addr.s_addr = INADDR_ANY; // Any IP
    client_address.sin_port = htons(PORT);       // Port setzen

    int addrlen = sizeof(struct sockaddr_in); // Adresse des Clients

    char buffer[BUFFER_SIZE] = {0}; // Buffer für Empfangene Daten


    // 2. Sockets erstellen
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {   // IPv4 und TCP-Verfahren
        std::cout << "server socket setup failed" << std::endl;
        return 1;
    }
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {   // IPv4 und TCP-Verfahren
        std::cout << "client socket setup failed" << std::endl;
        return 1;
    }

    // 3. Sockets an  Port binden
        if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {    // IPv4 und TCP-Verfahren
        std::cout << "server bind failed" << std::endl;
        return 1;
    }
    if (bind(client_socket, (struct sockaddr *)&client_address, sizeof(client_address)) < 0) {    // IPv4 und TCP-Verfahren
        std::cout << "client bind failed" << std::endl;
        return 1;
    }



    if(!firstUsr){
        //Establish connection with other user
        string serverIP;
        cout << "Server IP: ";
        cin >> serverIP;

        //
        inet_pton(AF_INET, serverIP.c_str(), &server_address.sin_addr);
        std::cout << "Verbindung wird hergestellt!" << std::endl;
        
        // Verbindung herstellen (connect)
        connect(client_socket, (struct sockaddr *)&client_address, sizeof(client_address));
        std::cout << "Verbindung hergestellt!" << std::endl;

        //initial handshake
        send(client_socket, ("INFO2 CONNECT/" + version + "\n\n").c_str(), strlen(buffer), 0);
        
        //receive handshake
        listen(server_socket, MAX_PENDING_CONNECTIONS);
        new_socket = accept(server_socket, (struct sockaddr *)&server_address, (socklen_t*)&addrlen);
        read(new_socket, buffer, BUFFER_SIZE);
        if(buffer != "INFO2 OK\n\n"){
            cout << "Error in handshake! Failed to establish connection!" <<endl;
            close(new_socket);
            close(server_socket);
            return -1;
        }
        else{
            cout << "Connection established!" <<endl;
            close(new_socket);
        }

    }

    /*
    // 4. Auf Verbindungen warten (listen)
    if (listen(server_fd, MAX_PENDING_CONNECTIONS) < 0) {
        std::cout << "listen failed" << std::endl;
        return 1;
    }
    for(int i = 0; i < MAX_PENDING_CONNECTIONS; i++){

        std::cout << "Server wartet auf Verbindungen auf " << "127.0.0.1" << ":" << PORT << std::endl;

        // 5. Verbindung annehmen (accept)
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            std::cout << "accept failed" << std::endl;
            return 1;
        }

        std::cout << "Verbindung akzeptiert!" << std::endl;
        
        // 6. Daten empfangen (recv)
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        std::cout << "Empfangene Nachricht: " << buffer << std::endl << std::endl;

        memset(buffer, 0, BUFFER_SIZE);

    }
    

    // 7. Sockets schließen
    close(new_socket);
    close(server_fd);
    */
    return 0;
}