#ifndef LISTEN_H
#define LISTEN_H

#pragma once
#include <string>
#include "socket.h"
#include <vector>

using namespace std;


SocketClss firstHandshakeHandler(string ownIP, double OwnVersion);
int listenHandler(SocketClss &socket, string ownIP, double OwnVersion, vector<string> &knownClients, vector<int> &MessageIDs);

#endif