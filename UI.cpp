#include <iostream>
#include "DataStorage.h"

void UI_Startup()
{
	char input;

	std::cout << "    ..........................................................\n";
	std::cout << "    .                                                        .\n";
	std::cout << "    .      INFO2 PROJECT from Lajos, Matthias, Andi          .\n";
	std::cout << "    .                                                        .\n";
	std::cout << "    .      hit [j] to join an existing P2P Network!          .\n";
	std::cout << "    .      hit [i] to create a new P2P Network!              .\n";
	std::cout << "    .                                                        .\n";
	std::cout << "    ..........................................................\n\n";
	std::cout << "Eingabe: ";

	Input:
	std::cin >> input;

	if (input == 'j')
	{
		//code
	}
	else if (input == 'i')
	{

		//code
	}
	else
	{
		std::cout << "Bitte nur j oder i eingeben! " << std::endl;
		goto Input;
	}
}

