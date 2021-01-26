#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <iostream>

enum Packet
{
	P_ChatMessage,
	P_Test
};

class Server
{
public:
	Server(int PORT, bool t_broadcastPublically = false);
	bool ListenForNewConnection();

private:
	bool sendall(int t_ID, char* t_data, int t_totalbytes);
	bool recvall(int t_ID, char* t_data, int t_totalbytes);

	bool SendInt(int t_ID, int t_int);
	bool GetInt(int t_ID, int& t_int);

	bool SendPacketType(int t_ID, Packet t_packettype);
	bool GetPacketType(int t_ID, Packet& t_packettype);

	bool SendString(int t_ID, std::string& t_string);
	bool GetString(int t_ID, std::string& t_string);

	bool ProcessPacket(int t_ID, Packet t_packettype);

	static void ClientHandlerThread(int t_ID);

private:
	SOCKET m_connections[100];
	int m_totalConnections = 0;

	SOCKADDR_IN m_addr; //Address that we will bind our listening socket to
	int m_addrlen = sizeof(m_addr);
	SOCKET m_sListen;
};

static Server* serverptr;
