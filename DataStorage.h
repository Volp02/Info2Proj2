#pragma once

#include <vector>
#include <iostream>
#include "socket.h"

using namespace std;

//stores the IP in the vector
bool storeIP(vector<string> &IPStr, string IP);

//returns an IP string from the vector
string giveIP(vector<string>& IPStr);

//returns if MessageID already exists (false = already exists)
bool checkMessageID(vector<int>& MessageID_Vector, int MessageID);

//creates a new MessageID and returns it
int createMessageID(vector<int> &MessageID_Vector);

//stores the MessageID in the vector
bool storeMessageID(vector<int> &MessageID_Vector, int MessageID);

//couts the number of IPs in the vector
int countIPs(vector<SocketClss> &IPStr);

//returns if MessageID already exists (false = already exist)
bool checkIP(vector<string> &knownClients, string IP);

