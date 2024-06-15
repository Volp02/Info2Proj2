#include "DataStorage.h"


#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "connect.h"
#include "listen.h"


#define PORT 26000
#define BUFFER_SIZE 1024


std::vector<std::string> storeIP(std::vector<std::string> &IPStr, const std::string IP)
{
	IPStr.push_back(IP);

	return IPStr;
}

bool storeMessageID(std::vector<int> &MessageID_Vector, int MessageID)
{
	MessageID_Vector.push_back(MessageID);
	return 0;
	
}

int createMessageID()
{
	srand(time(NULL));

	return rand() % 999999;
}

bool checkMessageID(std::vector<int>& MessageID_Vector, int MessageID)
{
	for (int i = 0; i < MessageID_Vector.size(); i++)
	{
		if (MessageID == MessageID_Vector[i])
			return 0;
		else
			return 1;
	}
}

std::string giveIP(std::vector<std::string>& IPStr)
{
	static int count = 1;

	if (IPStr.empty())
	{
		std::cout << "No ID's to pull from! " << std::endl;
		
	}
	if (IPStr.size() == 1)
	{
		return IPStr[0];
	}
	else
	{
		if (IPStr.size() > count)
		{
			std::string output = IPStr[IPStr.size() - count];
			count++;
			return output;
		}
	

		
	}


}

