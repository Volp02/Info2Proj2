#ifndef LISTEN_H
#define LISTEN_H

#pragma once
#include <string>
#include "socket.h"

SocketClss HandleFirstHandshake(SocketClss originalSocket, int Port, double OwnVersion);
int listenForIncomingConnection(SocketClss& socket, std::string ownIP, double OwnVersion, std::vector<SocketClss> &IPStr, std::vector<int> &MessageIDs);

#endif