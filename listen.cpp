#ifdef _WIN32 // Windows-spezifischer Code
#include "initWinsock.h"
#pragma comment(lib, "Ws2_32.lib")
typedef int socklen_t;
#else // Linux-spezifischer Code
#include "linuxLib.h"
#define closesocket close
#endif


#include <sstream>
#include <iostream>
#include <string.h> // Include the string.h header


#include "connect.h"
#include "DataStorage.h"
#include "socket.h"

//close  -  closesocketsocket
#define PORT 26000


using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;


int listenForIncomingConnection(SocketClss& socket, string ownIP, double OwnVersion, vector<SocketClss> &IPStr, vector<int> &MessageIDs)
{   
    
    socket.S_createAndBind(PORT); // create and bind socke
    cout << "bind done! ----"; 

    socket.S_listen(PORT);
    cout << "Warte auf Verbindungen..." << endl;
    
    // Unendliche Schleife, um mehrere Client-Verbindungen zu akzeptieren
    while (true)
    {
        SocketClss acceptSocket = socket.S_acceptConnection();
        // 6. recieve data
        char dataBuffer[1024] = {0};
        int recieveData = socket.receiveData(dataBuffer, 1024);

        {
            /* code */
        }
        string connectResponse(dataBuffer, 14);
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

                acceptSocket.sendData(acceptConnection); // Verwende sendData von MySocket
                std::cout << "responds with INFO2 OK! " << std::endl;

                //acceptSocket.closeSocket(); // Schließe den Server-Socket nach dem Handshake
                                                // Gib den Client-Socket zurück
            }
            else {
                std::cout << "handshake failed -> old version: " << clientVersion << endl;
            }
        }
        
        string BackconnectResponse (dataBuffer, 11);
        if (!(strcmp(BackconnectResponse.c_str(), "BACKCONNECT"))) {
            cout << "receved backConnection attempt" << endl;
            string responseIP(dataBuffer + 12);
            cout << responseIP;
            firstHandshake(responseIP, PORT ,OwnVersion);    
        }
        
        string FriendRqResponse(dataBuffer, 18);
        if (!(strcmp(BackconnectResponse.c_str(), "FRIEND REQUEST\n\n"))) {
            
            string IP;
            IP = giveIP(IPStr);
            socket.sendData(IP);
        }

        string SENDResponse(dataBuffer, 4);
        if (!(strcmp(SENDResponse.c_str(), "SEND"))) {
            
            string MessageToForward(dataBuffer);
            string RecevedMessageID (dataBuffer + 5, 11);
            std::stringstream ss3;
            ss3 << RecevedMessageID;
            int RecevedMessageIDint;
            ss3 >> RecevedMessageIDint;

            if (!checkMessageID(MessageIDs, RecevedMessageIDint)) {
                sendMessage(MessageToForward, RecevedMessageIDint, IPStr);
            }

            
        }

        cout << "whiling";
        
    } // Ende der Schleife

    // Server-Socket schließen (wird nie erreicht)
  
    socket.closeSocket();
    return 0;
} 
