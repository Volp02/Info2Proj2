#ifdef _WIN32 // Windows-spezifischer Code
#include "initWinsock.h"
typedef int socklen_t;
#else // Linux-spezifischer Code
#include "linuxLib.h"
#define closesocket close
#endif


#include "connect.h"
#include "socket.h"
#include "DataStorage.h"

#include <vector>
#include <sstream>

using namespace std;


bool firstHandshake(std::string IP, int Port, double OwnVersion, std::vector<std::string> &knownClients) {

    SocketClss ConnectSocket;
    std::cout << "connecting to server :" << IP << endl;
    ConnectSocket.C_createAndConnect(IP, Port); // connect to server


    if(ConnectSocket.handshakeOut(OwnVersion)){
        cout << "handshake successful" << endl;
        
        storeIP(knownClients,IP);
        ConnectSocket.closeSocket();
        return true;


    } // send handshake
    else{
        cout << "handshake not successful, returning" << endl;
        ConnectSocket.closeSocket();
        return false;
    }
}

bool sendMessageToClients(string Message, int MessageID, vector<string>& knownClients, int Port, double OwnVersion) {

    //cout << "--- im function sendMessage ---" << endl;
    //cout << "payload: " << Message << endl;

    if (knownClients.size() > 0) {

        for (int i = 0; i <= knownClients.size() - 1; i++) {

            SocketClss* SendSocket = new SocketClss();

            SendSocket->C_createAndConnect(knownClients[i], Port); // connect to server

            if (SendSocket->handshakeOut(OwnVersion)) {

                string sendPaload = "SEND " + to_string(MessageID) + ' ' + Message;

                SendSocket->sendData(sendPaload); // Senden der Nachricht)

            }

            SendSocket->closeSocket();

            delete SendSocket;

            cout << "send to: " << knownClients[i] << endl;

        }

        cout << "send to all known clients! " << endl;

        return true;

    }

    return false;
    
}