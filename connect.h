#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "socket.h"

using namespace std;

//Header for FirstTimeConnect

bool firstHandshake(string IP, int Port, double OwnVersion, vector<string> &knownClients);

bool backConnectSend(string ownIP, string sendIP);
string sendFriendRequest(string targetIP);

//sends the Message to all known Clients
bool sendMessageToClients(string Message, int MessageID, vector<string>& knownClients, int Port, double OwnVersion);

bool sendMessageIDlessDEBUG(string Message, vector<string> &knownClients);