#pragma once

#include <vector>
#include <iostream>
#include "socket.h"

bool storeIP(std::vector<SocketClss> &IPStr, const SocketClss& IP);
std::string giveIP(std::vector<SocketClss>& IPStr);
bool checkMessageID(std::vector<int>& MessageID_Vector, int MessageID);
int createMessageID(std::vector<int> &MessageID_Vector);
bool storeMessageID(std::vector<int> &MessageID_Vector, int MessageID);
int countIPs(std::vector<SocketClss> &IPStr);

void UI_Startup();
