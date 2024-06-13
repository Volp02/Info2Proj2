#include <sstream>
#include <iostream>
#include <string.h> // Include the string.h header

#include "initWinsock.h"
//#include "linuxLib.h"

#pragma comment(lib, "Ws2_32.lib")

using std::cin;
using std::cout;
using std::endl;
using std::string;

int listenForIncomingConnection()
{

    // 2. TCP Socket erstellen
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // 0 steht für das Standardprotokoll (TCP)

    // 3. Socket binden
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_port = htons(26000);
    inet_pton(AF_INET, "127.0.0.1", &service.sin_addr.s_addr);

    bind(serverSocket, (sockaddr *)&service, sizeof(service));

    // 4. Auf Verbindungen warten
    listen(serverSocket, 2); // 2: maximale Anzahl an wartenden Verbindungen

    cout << "Warte auf Verbindungen..." << endl;

    // Unendliche Schleife, um mehrere Client-Verbindungen zu akzeptieren
    while (true)
    {
        // 5. Verbindung akzeptieren
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int acceptSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientAddrLen);

        if (serverSocket == -1)
        {
            continue; // Zum nächsten Schleifendurchlauf springen
        }
        else
        {
           
            // 6. recieve data

            char dataBuffer[1024] = {0};
            double version = 0.6;
            bool accept;

            int recieveData = recv(acceptSocket, dataBuffer, 1024, 0);

            string response(dataBuffer, 13);

            if (!(strcmp(response.c_str(), "INFO2 CONNECT/")))
            {
                std::cout << "handshake failed\n";
                cout << response.c_str() << endl;
                continue;
            }
            std::stringstream ss2;
            string responseVers(dataBuffer + 14);
            ss2 << responseVers;
            double ServerVersion;
            ss2 >> ServerVersion;
            if (ServerVersion >= version)
            {
                std::cout << "handshake successful\n";
                accept = true;
            }
            else
            {
                accept = false;
                std::cout << "handshake failed" << endl;
                continue;
            }

            std::stringstream ss;
            string Sversion;
            ss << version;
            ss >> Sversion;

            if ((accept) == true)
            {

                cout << "Message: " << dataBuffer << endl;

                std::string acceptConnection = "INFO2 OK\n\n";

                send(acceptSocket, acceptConnection.c_str(), acceptConnection.length(), 0);
                cout << "sended: " << acceptConnection << endl;
                closesocket(acceptSocket);
            }
        }
    } // Ende der Schleife

    // Server-Socket schließen (wird in diesem Beispiel nie erreicht)
    cout << "while is closed" << endl;
    closesocket(serverSocket);
    return 0;
}
