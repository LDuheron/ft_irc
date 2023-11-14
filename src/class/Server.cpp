#include "../../private/Server.hpp"
#include <asm-generic/errno.h>
#include <cstddef>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <system_error>
#include <utility>

// Constructor -----------------------------------------------------------------

Server::Server() : //_allClients()
	_clientMap(), _allFd(),
	_epollFd(0), _epollEvent(), _IP(0),
	_nickname("DEFAULT"), _nbClients(0), _serverPassword("NULL"),
	_serverPort(0), _serverAddr(), _socket(0)
{
	if (DEBUG)
		std::cout << "Server : default constructor called.\n";
}

Server::Server(int port, std::string password) ://_allClients(),
	_clientMap(), _allFd(),
	_epollFd(0), _epollEvent(), _IP(0),
	_nickname("DEFAULT"), _nbClients(0), _serverPassword(password),
	_serverPort(port), _serverAddr(), _socket(0)
{
	if (DEBUG)
		std::cout << "Server : Param port and password constructor called.\n";
}

// Server::Server(Server const & src)
// {
// 	*this = src;
// 	if (DEBUG)
// 		std::cout << "Server : copy constructor called.\n";
// }

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
	this->_socket = newSocket;
}

// std::vector<Client*> const &	Server::getAllClients(void) const
// {
// 	return (this->_allClients);
// }

std::map<int, Client *> const &	Server::getClientMap(void) const
{
	return (this->_clientMap);
}

// Overload --------------------------------------------------------------------

// Server &	Server::operator=(Server const & rhs)
// {
// 	for (int i = 0; i < (int)rhs._allClients.size(); i++)
// 		this->_allClients.push_back(rhs._allClients[i]);
// 	for (int i = 0; i < (int)rhs._allFd.size(); i++)
// 		this->_allFd.push_back(rhs._allFd[i]);
// 	this->_epollFd = rhs._epollFd;
// 	this->_epollEvent = rhs._epollEvent;
// 	this->_IP = rhs._IP;
// 	this->_nickname = rhs._nickname;
// 	this->_nbClients = rhs._nbClients;
// 	this->_serverPassword = rhs._serverPassword;
// 	this->_serverPort = rhs._serverPort;
// 	this->_serverAddr = rhs._serverAddr;
// 	this->_socket = rhs._socket;
// 	return *this;
// }

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

	//epoll call
	this->_epollFd = epoll_create1(0);
	this->_eventArray[0].events = EPOLLIN | EPOLLOUT;
	this->_eventArray[0].data.fd = this->_socket;
	if (this->_epollFd == FAIL)
		std::cerr << "Error: Failed to epoll.\n";
	if (epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, this->_socket, &this->_eventArray[0]) == FAIL)
		std::cerr << "Error : Failed to add socket to epoll.\n";

	// Create socket
	setSocket(socket(AF_INET, SOCK_STREAM, 0));
	if(DEBUG2)
		perror("socket:");
	if (this->_socket == FAIL)
		std::cerr << "Error : Failed to create socket.\n";

	// Bind socket to IP / port
	if (bind(this->_socket, (sockaddr*)&(this->_serverAddr), sizeof(this->_serverAddr)) == FAIL)
		std::cerr << "Error : Failed to bind to port " << this->_serverPort << ".\n";
	if (DEBUG2)
		perror("bind:");

	// Listen on socket
	if (listen(this->_socket, MAX_CLIENTS) == FAIL)
		std::cerr << "Error : Failed to listen.\n";
	if (DEBUG2)
		perror("listen:");
	if (DEBUG2)
		std::cout << "Server initialisation successful.\n";
	this->_allFd.push_back(this->_socket);



	// RPL_WELCOME message

	// close(this->_epollFd);
	if (DEBUG)
	{
		std::cout << "server socket : " << this->_socket << std::endl;
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
	static int id = 0;

	if (DEBUG)
		std::cout << "Creating a new client\n";

	socklen_t	addrLen = sizeof(this->_serverAddr);
	int			clientSocket;

	// Attempt to accept a new client connection
	if ((clientSocket = accept(this->_socket, (struct sockaddr*)&(this->_serverAddr), &addrLen) == FAIL))
	{
		std::cerr << "Error : Failed to accept client connection.\n";
		close(clientSocket);
		return ;
	}
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
	Client *newClient = new Client(++id);
	newClient->setFd(clientSocket);
	this->_clientMap.insert(std::make_pair(newClient->getId(), newClient));
	this->_allFd.push_back(clientSocket);
	++this->_nbClients;

	//epoll call
	this->_eventArray[id].events = EPOLLIN | EPOLLOUT;
	this->_eventArray[id].data.fd = clientSocket;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, clientSocket, &this->_eventArray[id]) == FAIL)
		std::cerr << "Error : Failed to add socket to epoll.\n";
	std::cout << "New client connected : " << clientSocket << std::endl;
}

void	Server::handleNewRequest(void)
{}

void	Server::loop(void)
{
	check_inactivity();
	if (DEBUG)
		;// std::cout << "Enter in server loop.\n";

	/*
		Mettre un timeout pour epoll_wait ? Sinon le programme bloque en attendant un nouvel event
		mais on veut pouvoir checker les messages des clients deja connectes
		=> ouais bon ça marche moyen
	*/
	std::cout << "epoll_wait\n";
	int fd_ready = epoll_wait(this->_epollFd, this->_eventArray.data(), MAX_CLIENTS, 1);
	if (fd_ready == FAIL)
		std::cerr << "Error : Epoll_wait() failed.\n";
	for (int i = 0; i < fd_ready; i++)
	{
		if (this->_eventArray[0].data.fd == this->_socket)
			handleNewClient();
		else
			processMessages();
	}
	// if (epoll()) -> iterate on all fds stored in a vector
	// {
	// }
	// if ()

	/*
	should only be done if there are events in the epoll set
	-> check the events returned by epoll_wait and handle them accordingly
	-> if an event corresponds to the server socket (listening for
		new connections), then call accept inside the event handling.
	*/


	// else
	// handleNewRequest();

}

void	Server::processMessages(Client *client)
{

	char	buffer[MAX_MESSAGE_LENGTH];
	int		bytesRead;

	// for (std::map<int, Client *>::iterator it = this->_clientMap.begin(); it != this->_clientMap.end();)
	// {
	// 	Client *client = it->second;
	bytesRead = recv(client->getFd(), buffer, MAX_MESSAGE_LENGTH, 0);
	if (bytesRead < 0) // Make a function for this
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			// No data available yet, continue to the next client
			// std::cout << " ici \n";
			// continue;
		} else
		{
			/*
			- handle disconnection / errors
			- remove client from the list
			- can use vector erase here
			*/
			std::cerr << "Error : Failed to receive data from client" << client->getFd() << " : " << strerror(errno) << std::endl;
		}
		// std::cout << "Client disconnected: " << client->getFd() << std::endl;
	} else if (bytesRead == 0)
	{
		std::cout << "Client disconnected: " << client->getFd() << std::endl;
		// it = this->_clientMap.erase(it);
		// this->_allFd.erase(this->_allFd.begin() + i);
		this->_clientMap.erase(client->getId());
		delete client;
		this->_nbClients--;
	} else {
		buffer[bytesRead] = '\0';
		std::cout << "processing incoming message :\n" << buffer << std::endl;
		processMessage(client, buffer);
	}
	// }
}

void	Server::processMessage(Client *client, const std::string message)
{
	if (message.substr(0,6) == "CAP LS")
	{
		std::cout << "we there bro" << std::endl;
		if (send(client->getFd(), "001", 3, MSG_NOSIGNAL) == -1)
			std::cerr << "Error : Failed to send ack.\n";
		if (send(client->getFd(), "002", 3, MSG_NOSIGNAL) == -1)
			std::cerr << "Error : Failed to send ack.\n";
		if (send(client->getFd(), "003", 3, MSG_NOSIGNAL) == -1)
			std::cerr << "Error : Failed to send ack.\n";
		if (send(client->getFd(), "004", 3, MSG_NOSIGNAL) == -1)
			std::cerr << "Error : Failed to send ack.\n";
	}
	if (message.substr(0, 4) == "PING"){
		std::cout << "PING RECEIVED" << std::endl;
		handlePing(client->getFd(), message.substr(5));
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
