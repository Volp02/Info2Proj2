#ifndef LISTEN_H
#define LISTEN_H

#pragma once
#include <string>
#include "socket.h"

SocketClss HandleFirstHandshake(int Port, double OwnVersion);
int listenForIncomingConnection(SocketClss& socket, std::string ownIP, double OwnVersion, std::vector<std::string>& IPStr, std::vector<int>& MessageIDs);

#endif