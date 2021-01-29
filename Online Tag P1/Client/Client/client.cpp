#include "client.h"

Client::Client(std::string t_IP, int t_PORT)
{
	//Start winsocket
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup has failed to startup", "Error in startup", MB_OK | MB_ICONERROR);
		exit(0);
	}
	//get the ip of this machine
	m_addr.sin_addr.s_addr = inet_addr(t_IP.c_str()); 
	m_addr.sin_port = htons(t_PORT); 
	m_addr.sin_family = AF_INET; 
	//pointer for the client that will be used for client thread
	clientptr = this; 

}

bool Client::Connect()
{
	//setup the connection socket
	m_connection = socket(AF_INET, SOCK_STREAM, NULL); 
	//check if able to connect 
	if (connect(m_connection, (SOCKADDR*)&m_addr, m_sizeOfAddr) != 0) 
	{
		//if not able to connect
		MessageBoxA(NULL, "Failed Connection", "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	//if coneection was ok
	std::cout << "Managed to Connect" << std::endl;
	//create thread for the client that will recieve data server sends
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); 
	return true;
}
bool Client::SendPlayerData(std::string& t_string)
{
	if (!SendPacketType(PlayerData))
	{
		return false;
	}
	//find the length of string buffer
	int bufferLength = t_string.size();
	//send the length of the string buffer
	if (!SendInt(bufferLength))
	{
		//if failed return false
		return false;
	}
	//try send string buffer
	if (!SendAll((char*)t_string.c_str(), bufferLength))
	{
		//if this failed to send return false if not return true
		return false;
	}
	return true;

}

bool Client::CloseConnection()
{
	if (closesocket(m_connection) == SOCKET_ERROR)
	{
		//check for a error and make sure socket wasnt already closed
		if (WSAGetLastError() == WSAENOTSOCK)
		{
			//if was return true
			return true;
		}
		//message to state that the socket cannot close
		std::string ErrorMessage = "Cannot Close Socket as error " + std::to_string(WSAGetLastError()) + "ocurred";
		MessageBoxA(NULL, ErrorMessage.c_str(), "Error Ocured", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

std::string Client::GetPlayerData()
{
	return PlayerDataMsg;
}

bool Client::ProcessPacket(Packet t_packettype)
{
	switch (t_packettype)
	{
		case PlayerData:
		{
			//get the chat message and store in message
			if (!GetString(PlayerDataMsg))
			{
				//if failed to get cha message the return true
				return false;
			}
			//output the message to the user
		    std::cout << "Player Pos and Tagged Bool " << PlayerDataMsg << std::endl;
			break;
		}
		
	    //if the packet wasnt found output message
		default:
			std::cout << "packet is not recognised: " << t_packettype << std::endl; 
			break;
	}
	//returntrue
	return true;
}

void Client::ClientThread()
{
	Packet PacketType;
	while (true)
	{
		//get the packet type
		if (!clientptr->GetPacketType(PacketType))
		{
			//if theres a issue exit loop 
			break;
		}
		//process the packet type
		if (!clientptr->ProcessPacket(PacketType))
		{
			//if theres a issue exit loop 
			break;
		}
	}
	std::cout << "Lost connection toServer." << std::endl;
	//close the socket connection
	if (clientptr->CloseConnection()) 
	{
		//output this message if closed
		std::cout << "Socket to server closed." << std::endl;
	}
	//if the connection was unable to close output message below
	else 
	{
		std::cout << "Socket was unable to close." << std::endl;
	}
}

bool Client::SendAll(char* t_data, int t_totalbytes)
{
	//varaible to hold total number of bytes sent
	int bytesSent = 0; 
	//run as long as more bytes to be sent
	while (bytesSent < t_totalbytes) 
	{
		//try send remaining bytes
		int returnCheck = send(m_connection, t_data + bytesSent, t_totalbytes - bytesSent, NULL); 
		//check for socket error while sending bytes
		if (returnCheck == SOCKET_ERROR)
		{
			//return false if failed to send all
			return false;
		}
		//add onto bytes sent
		bytesSent += returnCheck; 
	}
	//retrurn true if it was successful
	return true; 
}

bool Client::SendInt(int t_int)
{
	//attempt to sent the int
	if (!SendAll((char*)&t_int, sizeof(int)))
	{
		//if it didnt send return false else return true
		return false;
	}
	return true; 
}

bool Client::SendPacketType(Packet t_packettype)
{
	//attempt to send the packet typr
	if (!SendAll((char*)&t_packettype, sizeof(Packet)))
	{
		//if it failed return false else return true
		return false;
	}
	return true; 
}

bool Client::RecieveAll(char* t_data, int t_totalbytes)
{
	//varaible to hold total number of bytes to be recieved
	int bytesReceived = 0;
	//run as long as bytes to be recieved
	while (bytesReceived < t_totalbytes) 
	{
		//try and to recieve bytes
		int returnCheck = recv(m_connection, t_data, t_totalbytes - bytesReceived, NULL); 
		//socket error while trying to recieve bytes
		if (returnCheck == SOCKET_ERROR)
		{
			//return false if failed to recieve
			return false;
		}
		//add onto bytes recieved
		bytesReceived += returnCheck; 
	}
	return true;
}

bool Client::GetInt(int& t_int)
{
	//attempt to recieve the int
	if (!RecieveAll((char*)&t_int, sizeof(int)))
	{
		//return false if failed to recieve all
		return false;
	}
	//return true if u managed to recieve all
	return true;
}

bool Client::GetPacketType(Packet& t_packettype)
{
	//try and recieve packet type
	if (!RecieveAll((char*)&t_packettype, sizeof(Packet)))
	{
		//return false if failed to recieve packet type and true if successful
		return false;
	}
	return true;
}

bool Client::GetString(std::string& t_string)
{
	//create variable to hold the length of the message
	int bufferLength; 
	//get the length of buffer and store in above variable
	if (!GetInt(bufferLength))
	{
		//return false if this failed
		return false; 
	}
	//create buffer of type char and assign
	char* buffer = new char[bufferLength + 1];
	//set the last character to be null
	buffer[bufferLength] = '\0';
	//recieve message and store in buffer array
	if (!RecieveAll(buffer, bufferLength)) 
	{
		//delete buffer and return false as it failed to recieve buffer
		delete[] buffer;
		return false; 
	}
	//ser the passed in string to be the recieved buffer message
	t_string = buffer; 
	//delete buffer and retrun true as we were successful retrieving string
	delete[] buffer; 
	return true;
}
