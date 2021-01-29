
#include "Server.h"

int main()
{
	//On port 100 create server
	Server MyServer(1111); 
	for (int i = 0; i < 100; i++) 
	{
		//create and accept up to 100 connections
		MyServer.listenForNewConnection(); 
	}
	system("pause");
	return 0;
}