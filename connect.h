#pragma once
#include <vector>
#include <iostream>

//Header for FirstTimeConnect

bool FirstTimeconnect(std::string firstIP, double version);
bool backConnectSend(std::string ownIP, std::string sendIP);
std::string sendFriendRequest(std::string targetIP);
bool sendMessage(std::string Message, int MessageID, std::vector<std::string> &knownClients);

bool sendMessageIDlessDEBUG(std::string Message, std::vector<std::string> &knownClients);