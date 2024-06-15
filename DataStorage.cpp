#include "DataStorage.h"


#include <iostream>
#include <string.h>
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

std::string giveID(std::vector<std::string>& IPStr)
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
		std::string output = IPStr[IPStr.size()-count];
		count++;
		return output;
	}


}

