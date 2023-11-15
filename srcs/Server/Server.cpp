#include "../Server/Server.hpp"
#include <asm-generic/errno.h>
#include <cstddef>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <system_error>
#include <utility>

// Constructor -----------------------------------------------------------------

Server::Server() : //_allClients(), //allFd(),
	_clientMap(), _epollFd(0), _serverEvent(), _IP(0),
	_nickname("DEFAULT"), _nbClients(0), _serverPassword("NULL"),
	_serverPort(0), _serverAddr(), _serverSocket(0)
{
	if (DEBUG)
		std::cout << "Server : default constructor called.\n";
}

Server::Server(int port, std::string password) ://_allClients(), allFd(),
	_clientMap(), _epollFd(0), _serverEvent(), _IP(0),
	_nickname("DEFAULT"), _nbClients(0), _serverPassword(password),
	_serverPort(port), _serverAddr(), _serverSocket(0)
{
	if (DEBUG)
		std::cout << "Server : Param port and password constructor called.\n";
}

// Destructor ------------------------------------------------------------------

Server::~Server()
{
	if (DEBUG)
		std::cout << "Server : destructor called.\n";
}

// Accessors -------------------------------------------------------------------

std::string const &	Server::getNickname(void) const
{
	return (this->_nickname);
}

std::string const &	Server::getPassword(void) const
{
	return (this->_serverPassword);
}

int const &	Server::getIP(void) const
{
	return (this->_IP);
}

int const &	Server::getPort(void) const
{
	return (this->_serverPort);
}

void	Server::setSocket(int newSocket)
{
	this->_serverSocket = newSocket;
}

std::map<int, Client *> const &	Server::getClientMap(void) const
{
	return (this->_clientMap);
}

// Overload --------------------------------------------------------------------

std::ostream & operator<<(std::ostream & lhs, Server const & rhs)
{
	lhs << "IP : " << rhs.getIP()
		<< "\nNickname : " << rhs.getNickname()
		<< "\nPassword : " << rhs.getPassword()
		<< "\nPort : " << rhs.getPort()
		<< "\nSocket : Unavailable"
		<< ".\n";
	return lhs;
}

// Functions - init data -------------------------------------------------------------------

// https://ncona.com/2019/04/building-a-simple-server-with-cpp/

// INIT_SERVERADDR : This function initializes the structure of data sockaddr_in.
// sin_family is AF_INET = gives to socket an IPv4 socket address to allow it to
// communicate with other hosts over a TCP/IP network.
// sin_port member defines the TCP/IP port number for the socket address.


void	Server::init_serverAddr(void)
{
	memset(&this->_serverAddr, 0, sizeof(this->_serverAddr));
	this->_serverAddr.sin_family = AF_INET;
	this->_serverAddr.sin_port = htons(this->_serverPort);
	// this->_serverAddr.sin_addr.s_addr = INADDR_ANY;
	this->_serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
}

void	Server::init_server(void)
{
	init_serverAddr();

	// Create socket
	setSocket(socket(AF_INET, SOCK_STREAM, 0));
	std::cout << "===== server socket : " << this->_serverSocket << " =====" << std::endl;
	if(DEBUG2)
		perror("socket:");
	if (this->_serverSocket == FAIL)
		std::cerr << "Error : Failed to create socket.\n";


	//epoll call
	this->_epollFd = epoll_create1(0);
	if (this->_epollFd == FAIL)
		std::cerr << "Error: Failed to epoll.\n";
	this->_serverEvent.events = EPOLLIN ; // EPOLLET for edge-triggered events ?
	this->_serverEvent.data.fd = this->_serverSocket;
	if (epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, this->_serverSocket, &this->_serverEvent) == FAIL)
		std::cerr << "Error : Failed to add socket to epoll.\n";

	// Bind socket to IP / port
	if (bind(this->_serverSocket, (sockaddr*)&(this->_serverAddr), sizeof(this->_serverAddr)) == FAIL)
		std::cerr << "Error : Failed to bind to port " << this->_serverPort << ".\n";
	if (DEBUG2)
		perror("bind:");

	// Listen on socket
	if (listen(this->_serverSocket, MAX_CLIENTS) == FAIL)
		std::cerr << "Error : Failed to listen.\n";
	if (DEBUG2)
		perror("listen:");
	if (DEBUG2)
		std::cout << "Server initialisation successful.\n";
	

	if (DEBUG)
	{
		std::cout << "server socket : " << this->_serverSocket << std::endl;
		std::cout << "port : " << this->_serverPort << std::endl;
	}
}

// Functions - launch server -------------------------------------------------------------------

void	Server::check_inactivity(void)
{
// 	send() send ping wait for pong?????????????
}

void	Server::handleNewClient(void)
{
	if (DEBUG)
		std::cout << "Creating a new client\n";

	socklen_t	addrLen = sizeof(this->_serverAddr);
	int			clientSocket;

	// Attempt to accept a new client connection
	if ((clientSocket = accept(this->_serverSocket, (struct sockaddr*)&(this->_serverAddr), &addrLen)) == FAIL)
	{
		std::cerr << "Error : Failed to accept client connection.\n";
		close(clientSocket);
		return ;
	}
	std::cout << "===== Client socket : " << clientSocket << " =====" << std::endl;
	if (DEBUG2)
		perror ("accept:");
	if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == FAIL)
	{
		std::cerr << "Error: Failed to configurate client socket in O_NONBLOCK mode.\n";
		close(clientSocket);
		return ;
	}
	if (this->_nbClients >= MAX_CLIENTS)
	{
		std::cerr << "Error : Too much clients connected.\n";
		close(clientSocket);
		return ;
	}
	if (clientSocket == FAIL)
		std::cerr << "Error : Failed to create socket.\n";
	Client *newClient = new Client(clientSocket);
	this->_clientMap.insert(std::make_pair(newClient->getSocket(), newClient));
	++this->_nbClients;

	//epoll call
	newClient->setEvent();
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, clientSocket, newClient->getEventAddress()) == FAIL)
		std::cerr << "Error : Failed to add socket to epoll.\n";
}

void	Server::handleNewRequest(void) //unnecessary ?
{}

void	Server::loop(void)
{

	if (DEBUG)
		std::cout << "Server loop started.\n";

	struct epoll_event events[MAX_CLIENTS];

	int fd_ready = epoll_wait(this->_epollFd, events, MAX_CLIENTS, -1); //timeout ?
	if (fd_ready == FAIL)
		std::cerr << "Error : Epoll_wait() failed.\n";

	std::cout << "fd_ready : " << fd_ready << std::endl;

	for (int i = 0; i < fd_ready; ++i)
	{
		int currentSocket = events[i].data.fd;
		std::cout << "currentSocket : " << currentSocket << std::endl;
		if (currentSocket == this->_serverSocket)
			handleNewClient();
		else
			handleClientEvent(this->_clientMap[currentSocket]);
	}
}

void	Server::handleClientEvent(Client *client)
{

	char	buffer[MAX_MESSAGE_LENGTH];
	int		bytesRead;

	bytesRead = recv(client->getSocket(), buffer, MAX_MESSAGE_LENGTH, 0);
	if (bytesRead < 0) // fct handleClientError
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK) // no data currently available, doesn't necessarily mean an error occured
			return;
		else
		{
			std::cerr << "Error : Failed to receive data from client " << client->getSocket() << " : " << strerror(errno) << std::endl;
			epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, client->getSocket(), client->getEventAddress());
			this->_clientMap.erase(client->getSocket()); // make fct removeClient
			delete client;
			this->_nbClients--;
		}
	} else if (bytesRead == 0) // fct handleClientDisconnect
	{
		std::cout << "Client disconnected: " << client->getSocket() << std::endl;
		epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, client->getSocket(), client->getEventAddress());
		this->_clientMap.erase(client->getSocket()); // make fct
		delete client;
		this->_nbClients--;
	} else {
		buffer[bytesRead] = '\0';
		std::cout << "processing incoming message :\n" << buffer << std::endl;
		processIncomingData(client, buffer);
	}
}

void	Server::processIncomingData(Client *client, const std::string message)
{
	/*
		Switch statement instead of awful if else (with function pointer beacuse it's cool)
	*/
	if (message.substr(0,6) == "CAP LS")
	{
		if (send(client->getSocket(), "001", 3, MSG_NOSIGNAL) == -1)
			std::cerr << "Error : Failed to send ack.\n";
		if (send(client->getSocket(), "002", 3, MSG_NOSIGNAL) == -1)
			std::cerr << "Error : Failed to send ack.\n";
		if (send(client->getSocket(), "003", 3, MSG_NOSIGNAL) == -1)
			std::cerr << "Error : Failed to send ack.\n";
		if (send(client->getSocket(), "004", 3, MSG_NOSIGNAL) == -1)
			std::cerr << "Error : Failed to send ack.\n";
	}
	if (message.substr(0, 4) == "PING"){
		std::cout << "PING RECEIVED" << std::endl;
		handlePing(client->getSocket(), message.substr(5));
	}
	/*
	else if (msg.substr(0, 4) == "JOIN")
	else if (msg.substr(0, 4) == "KICK")
	else if (msg.substr(0, 6) == "INVITE")
	else if (msg.substr(0, 5) == "TOPIC")
	else if (msg.substr(0, 4) == "MODE")
	*/
}

void	Server::handlePing(int clientSocket, const std::string &pingData)
{
	std::string pongResponse = ":localhost PONG :" + pingData + "\n";
	if (send(clientSocket, pongResponse.c_str(), pongResponse.length(), 0) == FAIL)
		std::cerr << "Error : Failed to send pong.\n";
}
