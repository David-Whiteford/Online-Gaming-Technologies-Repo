#include "Server.h"

Server::Server(int t_PORT, bool t_BroadcastPublically)
{
	//Start Winsock 
	WSAData WinSockData;
	WORD DllVer = MAKEWORD(2, 1);
	//if the wsaData is not zero then theres a error
	if (WSAStartup(DllVer, &WinSockData) != 0)
	{
		MessageBoxA(NULL, "The Winsock failed to start", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	//check server open to public
	if (t_BroadcastPublically)
	{
		m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		m_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	}
	//Port
	m_addr.sin_port = htons(t_PORT);
	//IPv4 Socket
	m_addr.sin_family = AF_INET; 
	//socket created to listen for new connections
	m_sListen = socket(AF_INET, SOCK_STREAM, NULL); 
	if (bind(m_sListen, (SOCKADDR*)&m_addr, sizeof(m_addr)) == SOCKET_ERROR) 
	{
		//Bind address to socket
		std::string ErrorMsg = "Failed to bind  address to listening socket with error" + std::to_string(WSAGetLastError());
		std::cout << ErrorMsg;
		exit(1);
	}
	//sListen socket is in a state where its listening for incoming connection. 
	if (listen(m_sListen, SOMAXCONN) == SOCKET_ERROR) 
	{
		std::string ErrorMsg = "Could not listen on the listening socket" + std::to_string(WSAGetLastError());
		std::cout << ErrorMsg;
		exit(1);
	}
	serverptr = this;

}

bool Server::ListenForNewConnection()
{
	//Accept a connection thats new
	SOCKET newConnection = accept(m_sListen, (SOCKADDR*)&m_addr, &m_addrlen);
	//check if the connection of the client was ok
	if (newConnection == 0) 
	{
		//if not output message
		std::cout << "Failed to accept the client's connection." << std::endl;
		return false;
	}
	else //otherwise
	{
		//ok message
		std::cout << "Client Connected And ID Is:" << m_totalConnections << std::endl;
		//set the socket in the array before creating thread
		m_connections[m_totalConnections] = newConnection; 
		//Thread created to handle this client. Has a index for this thread of i
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(m_totalConnections), NULL, NULL);  
		std::string message = "";
		SendGameData(m_totalConnections, message);
		//add one to the numbver of clients
		m_totalConnections++; 
		return true;
	}
}

bool Server::SendAll(int t_ID, char* t_data, int t_totalbytes)
{
	//total bytes sent
	int bytesSent = 0; 
	//while bytes have to be sent
	while (bytesSent < t_totalbytes) 
	{
		//try and send remaining bytes
		int ReturnCheck = send(m_connections[t_ID], t_data + bytesSent, t_totalbytes - bytesSent, NULL); 
		//check if there was a error
		if (ReturnCheck == SOCKET_ERROR)
		{
			//falied to send return false
			return false;
		}
		//add return check to total bytes
		bytesSent += ReturnCheck;
	}
	//managed to send bytes
	return true; 

}

bool Server::RecieveAll(int t_ID, char* t_data, int t_totalbytes)
{
	//has bytes recieved
	int bytesReceived = 0; 
	//run as long as there are bytes to recieve
	while (bytesReceived < t_totalbytes)
	{
		//attempt to recieve remaining bytes
		int ReturnCheck = recv(m_connections[t_ID], t_data + bytesReceived, t_totalbytes - bytesReceived, NULL);
		//check for error while recieveing bytes
		if (ReturnCheck == SOCKET_ERROR)
		{
			//then return false if couldnt recieve bytes
			return false;
		}
		bytesReceived += ReturnCheck;
	}
	//return true if successful when recieving bytes
	return true; 
}

bool Server::SendIntData(int t_ID, int t_int)
{
	bool SendState = false;
	SendState = SendAll(t_ID, (char*)&t_int, sizeof(int));
	if(SendState)
	{
		return true;
	}
	else
	{
		return false;
	}
	

}

bool Server::GetIntData(int t_ID, int& t_int)
{
	bool SendState = false;
	SendState = RecieveAll(t_ID, (char*)&t_int, sizeof(int));
	if (SendState)
	{
		return true;
	}
	else
	{
		return false;
	}
		
}

bool Server::SendPacketType(int t_ID, Packet t_packetType)
{
	bool SendState = false;
	SendState = SendAll(t_ID, (char*)&t_packetType, sizeof(Packet));
	if(SendState == false)
	{
		//return false if it failed and true if it didnt
		return false;
	}
	else
	{
		return true;
	}
}

bool Server::GetPacketType(int t_ID, Packet& t_packettype)
{
	bool SendState = false;
	//attempt to recieve packet
	SendState = RecieveAll(t_ID, (char*)&t_packettype, sizeof(Packet));
	if (SendState)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Server::SendGameData(int t_ID, std::string& t_string)
{
	bool SendState = false;
	SendState = SendPacketType(t_ID, PlayerData);
	int bufferlength = t_string.size();
	SendState = SendIntData(t_ID, bufferlength);	
	SendState = SendAll(t_ID, (char*)t_string.c_str(), bufferlength);
	if (SendState)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}


bool Server::SendPlayerData(int t_ID, std::string& t_string)
{
	bool SendState = false;
	SendState = SendPacketType(t_ID, PlayerData);
	int bufferlength = t_string.size();
	SendState = SendIntData(t_ID, bufferlength);
	SendState = SendAll(t_ID, (char*)t_string.c_str(), bufferlength);
	if (SendState)
	{
		return true;
	}
	else
	{
		return false;
	}


}
bool Server::GetThePlayerData(int t_ID, std::string& t_string)
{
	//length of buffer
	int bufferLength; 
	//get buffer length and store in variable bufferLength
	if (!GetIntData(t_ID, bufferLength))
	{
		//return false if this failes
		return false;
	}
	//create and assign buffer
	char* buffer = new char[bufferLength + 1];
	//last character of the buffer is null to save memory
	buffer[bufferLength] = '\0'; 
	//recieve and store message in buffer array
	if (!RecieveAll(t_ID, buffer, bufferLength))
	{
		//delete the buffer if this failed and return false
		delete[] buffer; 
		return false; 
	}
	//set the string to the recieve buffer message
	t_string = buffer;
	//then delete buffer and return true afer string retrieved
	delete[] buffer;
	return true;
}
bool Server::ProcessPacket(int t_ID, Packet t_packetType)
{
	//check the packet type
	switch (t_packetType)
	{
		//if its chat message
	case PlayerData: 
	{
		//create a string to store message
		std::string message; 
		bool getDataState = false;
		getDataState = GetThePlayerData(t_ID, message);
		if(getDataState == false)
		{
			//if this failed false returned
			return false;
		}
         //send  message out to each user
		for (int i = 0; i < m_totalConnections; i++)
		{
			//check that the message is the user that sent it
			if (i == t_ID)
			{
				//if it is then skip it so as to not sent message to themself
				continue;
			}
			//send the message to connection at index i
			bool sendDataStatus = false;
			sendDataStatus = SendGameData(i, message);
			if(sendDataStatus == false)
			{
				std::cout << "Could not send data form id: " << t_ID << " to client ID: " << i << std::endl;
			}
		}
		std::cout << "Recieved data from client with ID: " << t_ID << std::endl;
		break;
	}
	default: //If packet type is not accounted for
	{
		//output that packet wasnt found
		std::cout << "The packet was not rigth: " << t_packetType << std::endl;
		break;
	}
	}
	return true;
}

void Server::ClientHandlerThread(int t_ID)
{
	//set the packet type
	Packet packetType;
	while (true)
	{
		bool packetIsOk = false;
		//get the type of packet
		packetIsOk = serverptr->GetPacketType(t_ID, packetType);
			//then process the packet
		packetIsOk = serverptr->ProcessPacket(t_ID, packetType);
		if (packetIsOk == false)
		{
			break;
		}
	}
	//output message that states lost connection
	std::cout << "Connection was lost with client with a id of:  " << t_ID << std::endl;
	//close the clients socket at Id
	closesocket(serverptr->m_connections[t_ID]);
	return;
}
