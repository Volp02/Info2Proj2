#include "DataStorage.h"
#ifdef _WIN32 // Windows-spezifischer Code
#include "initWinsock.h"
typedef int socklen_t;
#else // Linux-spezifischer Code
#include "linuxLib.h"
#define closesocket close
#endif

#include <iostream>
#include <string.h>
#include <string>
#include <thread>
#include <vector>

#include "connect.h"
#include "listen.h"


#define PORT 26000
#define BUFFER_SIZE 1024


std::vector<std::string> storeIP(std::vector<std::string> &IPStr, std::string IP)
{
	IPStr.push_back(IP);

	return IPStr;
}

std::string giveID(std::vector<std::string>& IPStr, int count)
{
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

