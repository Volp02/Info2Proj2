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

// close  -  closesocketsocket
#define PORT 26000

using namespace std;

bool handleRequests(SocketClss &acceptSocket,string ownIP, double OwnVersion, vector<string> &knownClients, vector<int> &MessageIDs, int threadNum);

int listenHandler(string ownIP, double OwnVersion, vector<string> &knownClients, vector<int> &MessageIDs, int threadNum)
{
    cout << "tread " << threadNum << " started" << endl;

    SocketClss* serverSocket = new SocketClss();
    serverSocket->S_createAndBind(PORT);
    serverSocket->S_listen();


    SocketClss* acceptSocket = new SocketClss();

    acceptSocket = serverSocket->S_acceptConnection();

    

    char dataBuffer[1024];
    acceptSocket->receiveData(dataBuffer, 1024);

    //cout << "Received data: " << dataBuffer << endl;
    string handshakeResponse = acceptSocket->handshakeIn(dataBuffer, OwnVersion);
    cout << " respoinding ;" << handshakeResponse << endl;
    acceptSocket->sendData(handshakeResponse); // Handle Handshake

    // 6. recieve data

    string connectResponse(dataBuffer, 14);
    if (connectResponse == "INFO2 CONNECT/")
    {
        cout << "handshake Confirmed!"<< endl;
        memset(dataBuffer, 0, 1024);

        while (handleRequests(*acceptSocket, ownIP, OwnVersion, knownClients, MessageIDs, threadNum));
        

    } // Ende der Schleife
    acceptSocket->closeSocket();
    delete acceptSocket;

    serverSocket->closeSocket();
    delete serverSocket;

    cout << "Thread closed" << endl;
    return threadNum;
}

int listenThreading(string ownIP, double OwnVersion, vector<string> &knownClients, vector<int> &MessageIDs, int threadNum)
{

    for (int threadCount = 1; true; threadCount++) {

        
        threadCount = listenHandler(ownIP, OwnVersion, knownClients, MessageIDs, threadCount);

        if (threadCount > 0)
        {
            cout << "threadCount: " << threadCount << endl;
        }
        else
        {
            
            return 0;
        }

    }
}


bool handleRequests(SocketClss &acceptSocket,string ownIP, double OwnVersion, vector<string> &knownClients, vector<int> &MessageIDs, int threadNum)
{
    char dataBuffer[1024];
    cout << "ready for command" << endl;
    int BytesRecieved = acceptSocket.receiveData(dataBuffer, 1024);
    
    if (BytesRecieved > 0 && acceptSocket.sockfd != -1)
        {
            cout << "recieved ";

            string BackconnectResponse(dataBuffer, 11);
            string FriendRqResponse(dataBuffer, 18);
            string SENDResponse(dataBuffer, 4);
            string LogoffResponse(dataBuffer, 6);

            if (BackconnectResponse == "BACKCONNECT")
            {
                cout << "receved backConnection attempt" << endl;

                string responseIP(dataBuffer + 12);
                cout << responseIP;
                if (checkIP(knownClients, responseIP))
                {
                    acceptSocket.closeSocket();
                    firstHandshake(responseIP, PORT, OwnVersion, knownClients);
                }
                else
                    cout << responseIP << " is already a known client" << endl;

                cout << "end of Connection attempt" << endl;

                return true;
            }

            else if (FriendRqResponse == "FRIEND REQUEST\n\n")
            {
                cout << "friend request" << endl;

                string IP;
                IP = giveIP(knownClients);
                acceptSocket.sendData(IP);
                acceptSocket.closeSocket();

                cout << "end of Friendrequest" << endl;

                return true;
            }

            else if (SENDResponse == "SEND")
            {

                cout << "SEND" << endl;

                string MessageToForward(dataBuffer);
                string RecevedMessageID(dataBuffer + 5, 11);
                stringstream ss3;
                ss3 << RecevedMessageID;
                int RecevedMessageIDint;
                ss3 >> RecevedMessageIDint;

                cout << "messageID: " << RecevedMessageID;
                cout << " messageToForward: " << MessageToForward << endl;

                if (!checkMessageID(MessageIDs, RecevedMessageIDint)) // Check if messageID is already known
                {
                    storeMessageID(MessageIDs, RecevedMessageIDint);                                             // Adds messageID to already known messagIDs
                    sendMessageToClients(MessageToForward, RecevedMessageIDint, knownClients, PORT, OwnVersion); // forwars message to all clients
                }
                acceptSocket.closeSocket();

                cout << "end of SEND" << endl;

                return true;

            }
            else if (LogoffResponse == "LOGOFF") {

                acceptSocket.closeSocket();
                cout << "connection to terminated! " << endl;
                return false;

            }

            else
            {
                cout << "Request unknown: " << dataBuffer << endl;
                //acceptSocket.closeSocket();

                return true;
            }
        }

    else {
        
        if (BytesRecieved == 0) {
            cout << "empty second message" << endl;

        }
        else cout << "connection Lost" << endl;
       
        acceptSocket.closeSocket();
        return false;
    }

    return true;

}