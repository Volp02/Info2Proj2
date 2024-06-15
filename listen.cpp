#ifdef _WIN32 // Windows-spezifischer Code
#include "initWinsock.h"
typedef int socklen_t;
#else // Linux-spezifischer Code
#include "linuxLib.h"
#define closesocket close
#endif


#include <sstream>
#include <iostream>
#include <string.h> // Include the string.h header

#include "connect.h"

//close  -  closesocketsocket

#pragma comment(lib, "Ws2_32.lib")

using std::cin;
using std::cout;
using std::endl;
using std::string;

void listenForIncomingConnection(string ownIP, double OwnVersion)
{

    // 2. TCP Socket erstellen
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // 0 steht für das Standardprotokoll (TCP)

    // 3. Socket binden
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_port = htons(26000);
    inet_pton(AF_INET, ownIP.c_str(), &service.sin_addr.s_addr);

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
            int recieveData = recv(acceptSocket, dataBuffer, 1024, 0);

            string ConnectResponse(dataBuffer, 14);
            if (!(strcmp(ConnectResponse.c_str(), "INFO2 CONNECT/")))
            {
                cout << "receved Connection attempt" << endl;
                std::stringstream ss2;
                string responseVers(dataBuffer + 14);
                ss2 << responseVers;
                double clientVersion;
                ss2 >> clientVersion;
                if (clientVersion >= OwnVersion)
                {
                    std::cout << "handshake successful\n";
                    std::string acceptConnection = "INFO2 OK\n\n";

                    send(acceptSocket, acceptConnection.c_str(), acceptConnection.length(), 0);
                    cout << "responds with INFO2 OK! " << endl;
                    closesocket(acceptSocket);
                }
                else
                {
                    std::cout << "handshake failed" << endl;
                    continue;
                }

            }

            string BackconnectResponse (dataBuffer, 11);
            if (!(strcmp(BackconnectResponse.c_str(), "BACKCONNECT"))) {
                cout << "receved backConnection attempt" << endl;
                string responseIP(dataBuffer + 12);
                cout << responseIP;
                FirstTimeconnect(responseIP, OwnVersion);    
            }
            
            string FriendRqResponse(dataBuffer, 18);
            if (!(strcmp(BackconnectResponse.c_str(), "FRIEND REQUEST\n\n"))) {
                
                //string IP;
                //IP = getIP(); --> to be implemented...

                //send(acceptSocket, IP.c_str(), IP.length(), 0)
            }
            string SENDResponse(dataBuffer, 4);

        }
    } // Ende der Schleife

    // Server-Socket schließen (wird in diesem Beispiel nie erreicht)
  
    closesocket(serverSocket);
    return;
}
