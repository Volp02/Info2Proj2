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


// TO BE CHANGED TO USE CLASS: \/
/*
bool backConnectSend(SocketClss socket, string ownIP, string sendIP)
{
    cout << "--- im function backConnectSend ---" << endl;
    std::cout << "Backconnect to server " << sendIP << endl;

    string message = "BACKCONNECT " + ownIP;

    socket.sendData(message); // Senden der Nachricht)
    
    std::cout << "Backconnect: Nachricht gesendet: " << message << std::endl;
    return true;
}
string sendFriendRequest(SocketClss socket, std::string targetIP) {

    cout << "--- im function sendFriendRequest ---" << endl;
    std::cout << "sendFriendRequest: Verbunden zum Server " << targetIP << endl;


    string payload = "FRIEND REQUEST\n\n";

    socket.sendData(payload); // Senden der Nachricht)

    char buffer[12] = { 0 };

    socket.receiveData(buffer, 15);    

    string response(buffer);

    return response;
    
    // 5. Verbindung schließen
    
}
*/
bool sendMessageToClients(string Message, int MessageID, vector<string>& knownClients, int Port, double OwnVersion) {

    cout << "--- im function sendMessage ---" << endl;
    for (int i = 0; i <= sizeof(knownClients); i++) {

        SocketClss SendSocket;

        SendSocket.C_createAndConnect(knownClients[i], Port); // connect to server

        if(SendSocket.handshakeOut(OwnVersion)){

            string sendPaload;
            sendPaload = "SEND " + to_string(MessageID) + ' ' + Message;

            SendSocket.sendData(sendPaload); // Senden der Nachricht)

        }
        
        SendSocket.closeSocket();

    }

    return true;
}