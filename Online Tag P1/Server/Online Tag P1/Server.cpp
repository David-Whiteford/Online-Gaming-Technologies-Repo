#include "Server.h"

Server::Server(int PORT, bool BroadcastPublically)
{
	//Start Winsock 
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	//if the wsaData is not zero then theres a error
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	//check server open to public
	if (BroadcastPublically) 
		m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else //else server only for our router
		//locally broadcasted
		m_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	//Port
	m_addr.sin_port = htons(PORT); 
	//IPv4 Socket
	m_addr.sin_family = AF_INET; 
	//socket created to listen for new connections
	m_sListen = socket(AF_INET, SOCK_STREAM, NULL); 
	if (bind(m_sListen, (SOCKADDR*)&m_addr, sizeof(m_addr)) == SOCKET_ERROR) 
	{
		//Bind address to socket
		std::string ErrorMsg = "Failed to bind  address to listening socket result Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	//sListen socket is in a state where its listening for incoming connection. 
	if (listen(m_sListen, SOMAXCONN) == SOCKET_ERROR) 
	{
		std::string ErrorMsg = "Failed to listen on listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
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
		std::cout << "Client Connected! ID:" << m_totalConnections << std::endl;
		//set the socket in the array before creating thread
		m_connections[m_totalConnections] = newConnection; 
		//Thread created to handle this client. Has a index for this thread of i
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(m_totalConnections), NULL, NULL);  
		std::string MOTD = "MOTD: Welcome! This is the message of the day!.";
		SendString(m_totalConnections, MOTD);
		//add one to the numbver of clients
		m_totalConnections += 1; 
		return true;
	}
}

bool Server::sendall(int t_ID, char* t_data, int t_totalbytes)
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
			//falied to send return false
			return false; 
		//add return check to total bytes
		bytesSent += ReturnCheck;
	}
	//managed to send bytes
	return true; 

}

bool Server::recvall(int t_ID, char* t_data, int t_totalbytes)
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
			//then return false if couldnt recieve bytes
			return false; 
		bytesReceived += ReturnCheck;
	}
	//return true if successful when recieving bytes
	return true; 
}

bool Server::SendInt(int t_ID, int t_int)
{
	//Attempt to send int
	if (!sendall(t_ID, (char*)&t_int, sizeof(int)))
		//if failed return false else return true
		return false; 
	return true; 
}

bool Server::GetInt(int t_ID, int& t_int)
{
	//Attempt to recieve int
	if (!recvall(t_ID, (char*)&t_int, sizeof(int)))
		//failed to recieve int then return false else return true if successful
		return false; 
	return true;
}

bool Server::SendPacketType(int t_ID, Packet t_packetType)
{
	//try send the packet type
	if (!sendall(t_ID, (char*)&t_packetType, sizeof(Packet)))
		//return false if it failed and true if it didnt
		return false; 
	return true;
}

bool Server::GetPacketType(int t_ID, Packet& t_packettype)
{
	//attempt to recieve packet
	if (!recvall(t_ID, (char*)&t_packettype, sizeof(Packet))) 
		//false returned if it failed to receive packet else if it was succesful then true returned
		return false;
	return true;
}

bool Server::SendString(int t_ID, std::string& t_string)
{
	if (!SendPacketType(t_ID, ChatMessage))
		return false; 
	int bufferlength = t_string.size();
	if (!SendInt(t_ID, bufferlength))
		return false; 
	if (!sendall(t_ID, (char*)t_string.c_str(), bufferlength))
		return false; 
	return true; 
}


bool Server::SendPlayerData(int t_ID, std::string& t_string)
{
	if (!SendPacketType(t_ID, PlayerData))
		return false;
	int bufferlength = t_string.size();
	if (!SendInt(t_ID, bufferlength))
		return false;
	if (!sendall(t_ID, (char*)t_string.c_str(), bufferlength))
		return false;
	return true;

}
bool Server::GetString(int t_ID, std::string& t_string)
{
	//length of buffer
	int bufferLength; 
	//get buffer length and store in variable bufferLength
	if (!GetInt(t_ID, bufferLength)) 
		//return false if this failes
		return false; 
	//create and assign buffer
	char* buffer = new char[bufferLength + 1];
	//last character of the buffer is null to save memory
	buffer[bufferLength] = '\0'; 
	//recieve and store message in buffer array
	if (!recvall(t_ID, buffer, bufferLength)) 
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
bool Server::ProcessPacket(int t_ID, Packet t_packettype)
{
	//check the packet type
	switch (t_packettype)
	{
		//if its chat message
	case ChatMessage: 
	{
		//create a string to store message
		std::string message; 
		//get chat msg and store in variable above
		if (!GetString(t_ID, message))
			//if this failed false returned
			return false; 
         //send  message out to each user
		for (int i = 0; i < m_totalConnections; i++)
		{
			//check that the message is the user that sent it
			if (i == t_ID)
				//if it is then skip it so as to not sent message to themself
				continue;
			//send the message to connection at index i
			if (!SendString(i, message)) 
			{
				std::cout << "Failed to send message from client ID: " << t_ID << " to client ID: " << i << std::endl;
			}
		}
		std::cout << "Processed chat message packet from user ID: " << t_ID << std::endl;
		break;
	}
	case PlayerData:
	{
		std::string message;
		GetString(t_ID, message);
		for (int i = 0; i < m_totalConnections; i++)
		{
			if (i == t_ID){
			}
			else
			{
				SendString(i, message);
			}
		}
	}
	default: //If packet type is not accounted for
	{
		//output that packet wasnt found
		std::cout << "Unrecognized packet: " << t_packettype << std::endl; 
		break;
	}
	}
	return true;
}

void Server::ClientHandlerThread(int t_ID)
{
	Packet packetType;
	while (true)
	{
		//get the type of packet
		if (!serverptr->GetPacketType(t_ID, packetType))
			//if u cant get type exit loop
			break; 
		//then process the packet
		if (!serverptr->ProcessPacket(t_ID, packetType)) 
			//if u cant get type exit loop
			break; 
	}
	//output message that states lost connection
	std::cout << "Lost connection to client ID: " << t_ID << std::endl;
	//close the clients socket at Id
	closesocket(serverptr->m_connections[t_ID]);
	return;
}
