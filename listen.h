#ifndef LISTEN_H
#define LISTEN_H

#pragma once
#include <string>
#include "socket.h"
#include <vector>

using namespace std;


SocketClss firstHandshakeHandler(string ownIP, double OwnVersion);

int listenThreading(string ownIP, double OwnVersion, vector<string> &knownClients, vector<int> &MessageIDs, int threadNum);
int listenHandler(string ownIP, double OwnVersion, vector<string> &knownClients, vector<int> &MessageIDs, int threadNum);

string checkForHandsake(char dataBuffer[], double OwnVersion);

#endif