#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")

#include <WinSock2.h>
#include <string>
#include <iostream>

enum Packet
{
	PlayerData
};

class Server
{
public:
	Server(int t_PORT, bool t_BroadcastPublically = false);
	bool listenForNewConnection();

private:
	bool sendAll(int t_ID, char* t_data, int t_totalbytes);
	bool recieveAll(int t_ID, char* t_data, int t_totalbytes);

	bool sendIntData(int t_ID, int t_int);
	bool getIntData(int t_ID, int& t_int);

	bool sendPacketType(int t_ID, Packet t_packettype);
	bool getPacketType(int t_ID, Packet& t_packettype);

	bool sendGameData(int t_ID, std::string& t_string);
	bool sendPlayerData(int t_ID, std::string& t_string);
	bool getThePlayerData(int t_ID, std::string& t_string);

	bool processPacket(int t_ID, Packet t_packetType);

	static void clientHandlerThread(int t_ID);

private:
	SOCKET m_connections[100];
	int m_totalConnections = 0;

	SOCKADDR_IN m_addr; //Address that we will bind our listening socket to
	int m_addrlen = sizeof(m_addr);
	SOCKET m_sListen;
};

static Server* serverptr;
