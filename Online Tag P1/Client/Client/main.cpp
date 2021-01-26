#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 


#include "Game.h"
#include "Client.h"


int main()
{
	//create a client on local host below and on port 1111
	Client myClient("127.0.0.1", 1111); 
	//check if the client can connect
	if (!myClient.Connect()) 
	{
		//output message if failed to connect to server and pause system
		std::cout << "Failed connection to the server" << std::endl;
		system("pause");
		return 1;
	}

	//create a string to hold user message
	std::string userInput;
	while (true)
	{
		//if the user preses enter get the line and fill buffer
		std::getline(std::cin, userInput); 
		//send the string user input 
		if (!myClient.SendString(userInput))
		{
			//if this failes then break from the loop
			break; 
		}
			
		Sleep(10);
	}
	Game game;
	game.run();

	system("pause");
	return 0;
}

