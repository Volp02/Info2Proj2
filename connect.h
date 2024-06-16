#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "socket.h"

//Header for FirstTimeConnect

SocketClss firstHandshake(std::string IP, int Port, double OwnVersion);
bool backConnectSend(std::string ownIP, std::string sendIP);
std::string sendFriendRequest(std::string targetIP);
bool sendMessage(std::string Message, int MessageID, std::vector<SocketClss>& knownClients);

bool sendMessageIDlessDEBUG(std::string Message, std::vector<std::string> &knownClients);