#pragma once
#include <string>

class ListeningSocket {
public:
    ListeningSocket(int port);
    bool createAndBind();
    bool listenForConnections(int backlog = 10);
    int acceptConnection();
    void closeSocket();

private:
    int port;
    int sockfd;
    struct sockaddr_in serverAddr;
};
