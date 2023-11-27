#include "../Server/Server.hpp"
#include <asm-generic/errno.h>
#include <cstddef>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <system_error>
#include <utility>

// Constructor -----------------------------------------------------------------

Server::Server() :
	_clientMap(),
	_epollSocket(0),
	_serverEvent(),
	_IP(0),
	_nickname("DEFAULT"),
	_nbClients(0),
	_serverPassword("NULL"),
	_serverPort(0),
	_serverAddr(),
	_serverSocket(0)
{
	if (DEBUG)
		std::cout << "Server : default constructor called.\n";
}

Server::Server(int port, std::string password) :
	_clientMap(),
	_epollSocket(0),
	_serverEvent(),
	_IP(0),
	_nickname("DEFAULT"),
	_nbClients(0),
	_serverPassword(password),
	_serverPort(port),
	_serverAddr(),
	_serverSocket(0)
{}

// Destructor ------------------------------------------------------------------

Server::~Server()
{
	for (std::map<int, Client *>::iterator it = this->_clientMap.begin(); it != this->_clientMap.end(); ++it)
		delete it->second;
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
	this->_serverAddr.sin_addr.s_addr = INADDR_ANY;
}

static int createSocket(void)
{
	int newSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (newSocket == -1)
	{
		std::perror("Error: Failed to create socket");
		std::exit(-1);
	}
	return (newSocket);
}

static int createEpoll(void)
{
	int newEpoll = epoll_create1(0);
	if (newEpoll == -1)
	{
		std::perror("Error: Failed to epoll");
		std::exit(-1);
	}
	return (newEpoll);
}

static void	controllEpoll(int epollSocket, int operation, int socket, struct epoll_event *event)
{
	if (epoll_ctl(epollSocket, operation, socket, event) == -1)
	{
		std::perror("Error: Failed to control epoll");
		std::exit(-1);
	}
}

static void	bindSocket(int socket, const struct sockaddr *addr)
{
	if (bind(socket, (struct sockaddr *)addr, sizeof(*addr)) == -1)
	{
		std::perror("Error: Failed to bind socket");
		std::exit(-1);
	}
}

static void	listenSocket(int socket)
{
	if (listen(socket, MAX_CLIENTS) == -1)
	{
		std::perror("Error: Failed to listen socket");
		std::exit(-1);
	}
}

void	Server::init_server(void)
{
	init_serverAddr();

	// Create socket
	setSocket(createSocket());
	if (DEBUG2)
		std::cout << "===== server socket : " << this->_serverSocket << " =====" << std::endl;

	//epoll call
	this->_epollSocket = createEpoll();
	this->_serverEvent.events = EPOLLIN ; // EPOLLET for edge-triggered events ?
	this->_serverEvent.data.fd = this->_serverSocket;
	controllEpoll(this->_epollSocket, EPOLL_CTL_ADD, this->_serverSocket, &this->_serverEvent);

	// Bind socket to IP / port
	bindSocket(this->_serverSocket, (sockaddr *)&(this->_serverAddr));

	// Listen on socket
	listenSocket(this->_serverSocket);
}

// Functions - launch server -------------------------------------------------------------------

static int	acceptNewClient(int serverSocket, struct sockaddr_in serverAddr)
{
	int			clientSocket;
	socklen_t	addrLen = sizeof(serverAddr);

	if ((clientSocket = accept(serverSocket, (struct sockaddr*)&(serverAddr), &addrLen)) == FAIL)
	{
		std::perror("Error : Failed to accept client connection.\n");
		close(clientSocket);
		return -1;
	}
	return (clientSocket);
}

static void	controlSocket(int socket, int operation)
{
	if (fcntl(socket, F_SETFL, operation) == FAIL)
	{
		std::perror("Error: Failed to control socket");
		close(socket);
	}
}

static Client	*storeClient(int clientSocket, std::map<int, Client *> &clientMap, int &nbClients)
{
	if (nbClients >= MAX_CLIENTS)
	{
		std::cerr << "Error : Too much clients connected.\n";
		close(clientSocket);
		return NULL;
	}

	Client *newClient = new Client(clientSocket);
	clientMap.insert(std::make_pair(newClient->getSocket(), newClient));
	++nbClients;
	return newClient;
}

void	Server::handleNewClient(void)
{
	int			clientSocket;

	clientSocket = acceptNewClient(this->_serverSocket, this->_serverAddr);
	if (clientSocket == -1)
		return ;
	if (DEBUG2)
		std::cout << "===== Client socket : " << clientSocket << " =====" << std::endl;
	
	// Set client socket to non-blocking mode
	controlSocket(clientSocket, O_NONBLOCK);

	Client *newClient = storeClient(clientSocket, this->_clientMap, this->_nbClients);

	controllEpoll(this->_epollSocket, EPOLL_CTL_ADD, clientSocket, newClient->getEventAddress());
}

void	Server::loop(void)
{
	int					fd_ready;
	struct epoll_event	events[MAX_CLIENTS];

	if ((fd_ready = epoll_wait(this->_epollSocket, events, MAX_CLIENTS, -1)) == -1)
		std::perror("Error: Failed to wait for events");

	for (int i = 0; i < fd_ready; ++i)
	{
		int currentSocket = events[i].data.fd;
		if (currentSocket == this->_serverSocket)
			handleNewClient();
		else
			handleClientEvent(this->_clientMap[currentSocket]);
	}
}

void	Server::removeClient(Client *client)
{
	epoll_ctl(this->_epollSocket, EPOLL_CTL_DEL, client->getSocket(), client->getEventAddress());
	this->_clientMap.erase(client->getSocket()); // make fct removeClient
	delete client;
	this->_nbClients--;
}

void	Server::handleClientError(Client *client)
{
	if (errno == EAGAIN || errno == EWOULDBLOCK) // no data currently available, doesn't necessarily mean an error occured
		return;
	else
	{
		std::perror("Error : Failed to receive data");
		removeClient(client);
	}
}

void	Server::handleClientDisconnection(Client *client)
{
	std::cout << "Client disconnected: " << client->getSocket() << std::endl;
	removeClient(client);
}

void	Server::handleClientEvent(Client *client)
{
	char	buffer[MAX_MESSAGE_LENGTH];
	int		bytesRead;

	bytesRead = recv(client->getSocket(), buffer, MAX_MESSAGE_LENGTH, 0);
	if (bytesRead < 0)
		handleClientError(client);
	else if (bytesRead == 0)
		handleClientDisconnection(client);
	else
	{
		buffer[bytesRead] = '\0';
		std::cout << "===== incoming message : =====\n" << buffer << std::endl;
		processIncomingData(client, buffer);
	}
}

void	Server::processIncomingData(Client *client, const std::string message)
{
	/*
		Switch statement instead of awful if else (with function pointer beacuse it's cool)
	*/
	std::string capLs = ":DEFAULT CAP * LS :none cause i dont care\n";
	std::string welcome1 = ":DEFAULT 001 lletourn :Welcome to the Internet Relay Network lletourn!user@host\n";
	std::string welcome2 = ":DEFAULT 002 lletourn :Your host is DEFAULT, running version 0.1\n";
	std::string welcome3 = ":DEFAULT 003 lletourn :This server was created 2023/11/20\n";
	std::string welcome4 = ":DEFAULT 004 lletourn DEFAULT ircd_version user_modes chan_modes\n";
	std::string pass = "Password required :\n";
	if (message.substr(0,6) == "CAP LS")
	{
		if ((send(client->getSocket(), capLs.c_str(), capLs.length(), MSG_NOSIGNAL)) == -1)
			std::cerr << "Error : Failed to send CAP LS answer.\n";
	}
	else if (message.substr(0,7) == "CAP END")
	{
		if (send(client->getSocket(), welcome1.c_str(), welcome1.length(), MSG_NOSIGNAL) == -1)
			std::cerr << "Error : Failed to send 001.\n";
		if (send(client->getSocket(), welcome2.c_str(), welcome2.length(), MSG_NOSIGNAL) == -1)
			std::cerr << "Error : Failed to send 002.\n";
		if (send(client->getSocket(), welcome3.c_str(), welcome3.length(), MSG_NOSIGNAL) == -1)
			std::cerr << "Error : Failed to send 003.\n";
		if (send(client->getSocket(), welcome4.c_str(), welcome4.length(), MSG_NOSIGNAL) == -1)
			std::cerr << "Error : Failed to send 004.\n";
		
		// std::cout << "USER RECEED : " << client->getSocket() << std::endl;
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
