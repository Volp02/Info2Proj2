#pragma once

#include <vector>
#include <iostream>

std::vector<std::string> storeIP(std::vector<std::string>& IPStr, const std::string IP);
std::string giveIP(std::vector<std::string>& IPStr, int count);
bool checkMessageID(std::vector<int>& MessageID_Vector, int MessageID);
int createMessageID();
