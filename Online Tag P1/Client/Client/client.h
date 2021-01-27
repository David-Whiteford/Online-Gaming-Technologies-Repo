#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib") 
#include <WinSock2.h> 
#include <string> 
#include <iostream> 

enum Packet
{
	ChatMessage,
	PlayerData
};

class Client
{
public:
	Client(std::string t_IP, int t_PORT);
	bool Connect();
	
	bool SendString(std::string& t_string);
	bool SendPlayerData(std::string& t_string);
	bool CloseConnection();

	bool ProcessPacket(Packet t_packettype);
	static void ClientThread();
	
	bool SendAll(char* t_data, int t_totalbytes);
	bool SendInt(int t_int);
	bool SendPacketType(Packet t_packettype);
	
	bool RecieveAll(char* t_data, int t_totalbytes);
	bool GetInt(int& t_int);
	bool GetPacketType(Packet& t_packettype);
	bool GetString(std::string& t_string);

private:
	SOCKET m_connection;
	SOCKADDR_IN m_addr;
	int m_sizeOfAddr = sizeof(m_addr); 
	std::string PlayerDataMsg;
};

static Client* clientptr; 