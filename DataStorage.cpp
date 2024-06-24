#include "DataStorage.h"

#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "connect.h"
#include "listen.h"
#include "socket.h"

#define PORT 26000
#define BUFFER_SIZE 1024

using namespace std;

bool storeIP(vector<string> &IPStr, string IP)
{
	IPStr.push_back(IP);

	return true;
}

//returns if IP already exists (false = already exists)
bool checkIP(vector<string>& knownClients, string IP) {
	if (knownClients.empty()) {
		return true;
	}
	for (int i = knownClients.size() - 1; i >= 0; i--) {
		if (knownClients[i] == IP) {
			return false; //return false: IP already exists
		}
	}
	return true; //return true: IP doesn't exist
}


bool storeMessageID(vector<int> &MessageID_Vector, int MessageID)
{
	cout << "--- im function storeMessageID ---" << endl;
	MessageID_Vector.push_back(MessageID);
	return 1;
}

int createMessageID(vector<int> &MessageID_Vector)
{
	cout << "--- im function createMessageID ---" << endl;
	int msgID;
	do
	{
		srand(time(NULL));
		msgID = rand() % 999999;
	} while (!checkMessageID(MessageID_Vector, msgID));

	storeMessageID(MessageID_Vector, msgID);
	return msgID;
}

bool checkMessageID(vector<int> &MessageID_Vector, int MessageID)
{
	cout << "--- im function checkMessageID ---" << endl;
	for (int i = 0; i < MessageID_Vector.size(); i++)
	{
		if (MessageID == MessageID_Vector[i])
			return 0;
	}
	return true;
}

int countIPs(vector<SocketClss> &IPStr){
	return IPStr.size();
}

string giveIP(vector<string> &IPStr)
{	
	cout << "--- im function giveIP ---" << endl; 
	static int count = 1;

	if (IPStr.empty())
	{
		cout << "No ID's to pull from! " << endl;
		return "0.0.0.0";
	}
	if (IPStr.size() == 1)
	{
		return IPStr[0];
	}
	else
	{
		if (IPStr.size() > count)
		{
			string output = IPStr[IPStr.size() - count];
			count++;
			return output;
		}
	}
	return "0";
}
