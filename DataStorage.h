#pragma once

#include <vector>
#include <iostream>
#include "socket.h"

std::vector<SocketClss> storeIP(std::vector<SocketClss> &IPStr, const SocketClss& IP);
std::string giveIP(std::vector<SocketClss>& IPStr);
bool checkMessageID(std::vector<int>& MessageID_Vector, int MessageID);
int createMessageID(std::vector<int> &MessageID_Vector);

