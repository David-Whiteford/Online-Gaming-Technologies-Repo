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
	bool ListenForNewConnection();

private:
	bool SendAll(int t_ID, char* t_data, int t_totalbytes);
	bool RecieveAll(int t_ID, char* t_data, int t_totalbytes);

	bool SendIntData(int t_ID, int t_int);
	bool GetIntData(int t_ID, int& t_int);

	bool SendPacketType(int t_ID, Packet t_packettype);
	bool GetPacketType(int t_ID, Packet& t_packettype);

	bool SendGameData(int t_ID, std::string& t_string);
	bool SendPlayerData(int t_ID, std::string& t_string);
	bool GetThePlayerData(int t_ID, std::string& t_string);

	bool ProcessPacket(int t_ID, Packet t_packetType);

	static void ClientHandlerThread(int t_ID);

private:
	SOCKET m_connections[100];
	int m_totalConnections = 0;

	SOCKADDR_IN m_addr; //Address that we will bind our listening socket to
	int m_addrlen = sizeof(m_addr);
	SOCKET m_sListen;
};

static Server* serverptr;
