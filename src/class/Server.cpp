#include "../../private/Server.hpp"
#include <cstddef>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <system_error>

// Constructor -----------------------------------------------------------------

Server::Server() :
	_allClients(), _allFd(),
	_epollFd(0), _epollEvent(), _IP(0),
	_nickname("DEFAULT"), _nbClients(0), _password("NULL"),
	_port(0), _serverAddr(), _socket(0)
{
	if (DEBUG)
		std::cout << "Server : default constructor called.\n";
}

Server::Server(int port, std::string password) :
	_allClients(), _allFd(),
	_epollFd(0), _epollEvent(), _IP(0),
	_nickname("DEFAULT"), _nbClients(0), _password(password),
	_port(port), _serverAddr(), _socket(0)
{
	if (DEBUG)
		std::cout << "Server : Param port and password constructor called.\n";
}

Server::Server(Server const & src)
{
	*this = src;
	if (DEBUG)
		std::cout << "Server : copy constructor called.\n";
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
	return (this->_password);
}

int const &	Server::getIP(void) const
{
	return (this->_IP);
}

int const &	Server::getPort(void) const
{
	return (this->_port);
}

void	Server::setSocket(int newSocket)
{
	this->_socket = newSocket;
}

std::vector<Client*> const &	Server::getAllClients(void) const
{
	return (this->_allClients);
}

// Overload --------------------------------------------------------------------

Server &	Server::operator=(Server const & rhs)
{
	for (int i = 0; i < (int)rhs._allClients.size(); i++)
		this->_allClients.push_back(rhs._allClients[i]);
	for (int i = 0; i < (int)rhs._allFd.size(); i++)
		this->_allFd.push_back(rhs._allFd[i]);
	this->_epollFd = rhs._epollFd;
	this->_epollEvent = rhs._epollEvent;
	this->_IP = rhs._IP;
	this->_nickname = rhs._nickname;
	this->_nbClients = rhs._nbClients;
	this->_password = rhs._password;
	this->_port = rhs._port;
	this->_serverAddr = rhs._serverAddr;
	this->_socket = rhs._socket;
	return *this;
}

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
	this->_serverAddr.sin_port = htons(this->_port);
	// this->_serverAddr.sin_addr.s_addr = INADDR_ANY;
	this->_serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
}

void	Server::init_server(void)
{
	init_serverAddr();
	this->_epollFd = epoll_create1(0);
	if (this->_epollFd == FAIL)
		std::cerr << "Error: Failed to epoll.\n";

	setSocket(socket(AF_INET, SOCK_STREAM, 0));
	std::cout << "socket : " << this->_socket << std::endl;
	perror("socket:");
	if (this->_socket == FAIL)
		std::cerr << "Error : Failed to create socket.\n";

	// setsocktopt() ???

	// Server should not be in non-blocking mode ?
	// ==> the accept() function will block until a client connects to the server
	// if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) == FAIL)
	// 	std::cerr << "Error: Failed to configurate fd in O_NONBLOCK mode.\n";

	if (bind(this->_socket, (sockaddr*)&(this->_serverAddr), sizeof(this->_serverAddr)) == FAIL)
		std::cerr << "Error : Failed to bind to port " << this->_port << ".\n";
	std::cout << "bind : " << this->_socket << std::endl;
	perror("bind:");
	if (listen(this->_socket, MAX_CLIENTS) == FAIL)
		std::cerr << "Error : Failed to listen.\n";
	std::cout << "listen : " << this->_socket << std::endl;
	perror("listen:");
	if (DEBUG)
		std::cout << "Server initialisation successful.\n";

	this->_allFd.push_back(this->_socket);

////////// test rapidos
	this->_epollEvent.events = EPOLLIN;
	this->_epollEvent.data.fd = this->_socket;

	if (epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, this->_socket, &this->_epollEvent) == FAIL)
		std::cerr << "Error : Failed to add socket to epoll.\n";
//////////

	// RPL_WELCOME message

	// close(this->_epollFd);
	std::cout << "server socket : " << this->_socket << std::endl;
	std::cout << "port : " << this->_port << std::endl;
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

	// Client *newClient = new Client;
	/*
	Need to figure out why accept returns -1:
	- socket not properly initialized ?
		-> check socket() bind() and listen() return values
	*/
	// int clientSocket = accept(this->_socket, (struct sockaddr*)&(this->_serverAddr), (socklen_t*)sizeof(this->_serverAddr));
	int clientSocket = accept(this->_socket, NULL, NULL);
	std::cout << "new client fd : " << clientSocket << std::endl;
	perror("accept:");

	// Attempt to accept a new client connection
	if (clientSocket == FAIL)
	{
		std::cerr << "Error : Failed to accept client connection.\n";
		close(clientSocket);
		return ;
	}
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
	Client *newClient = new Client;
	newClient->setFd(clientSocket);
	this->_allClients.push_back(newClient);
	this->_allFd.push_back(clientSocket);
	this->_nbClients += 1;
}

void	Server::handleNewRequest(void)
{
	// recv ?

	// if (bind(this->_socket, (sockaddr*)&(this->_serverAddr), sizeof(this->_serverAddr)) == FAIL)
	// 	std::cerr << "Error : Failed to bind to port " << this->_port << ".\n";

	// if (listen(this->_socket, MAX_CLIENTS) == FAIL)
	// 	std::cerr << "Error : Failed to listen.\n";

	// if (accept(this->_socket, (sockaddr*)&(this->_serverAddr), sizeof(this->_serverAddr)) == FAIL)
	// 	std::cerr << "Error : Failed to accept.\n";
}

	// if (connect(this->_socket, (sockaddr*)&(this->_serverAddr), sizeof(this->_serverAddr)) == FAIL)
	// 	std::cerr << "Error : Failed to connect.\n";

void	Server::loop(void)
{
	check_inactivity();
	if (DEBUG)
		std::cout << "Enter in server loop.\n";

	int fd_ready = epoll_wait(this->_epollFd, &this->_epollEvent, MAX_CLIENTS, -1);
	if (fd_ready == FAIL)
		std::cerr << "Error : Epoll_wait() failed.\n";
	for (int i = 0; i < fd_ready; i++)
	{
		if (this->_epollEvent.data.fd == this->_socket){
			handleNewClient();
			std::cout << "New client connected.\n";
		}
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

void	Server::processMessages()
{

	char	buffer[MAX_MESSAGE_LENGTH];
	int		bytesRead;

	for (size_t i = 0; i < this->_allClients.size(); ++i)
	{
		Client *client = this->_allClients[i];
		bytesRead = recv(client->getFd(), buffer, MAX_MESSAGE_LENGTH, 0);
		if (bytesRead <= 0)
		{
			/*
			- handle disconnection / errors
			- remove client from the list
			- can use vector erase here
			*/
		} else
		{
			buffer[bytesRead] = '\0';
			std::cout << " PROCESSING MESSAGE " << std::endl;
			processMessage(client, buffer);
		}
	}
}

void	Server::processMessage(Client *client, const char *message)
{
	std::string msg = message;
	if (msg.substr(0, 4) == "PING"){
		std::cout << "PING RECEIVED" << std::endl;
		handlePing(client->getFd(), msg.substr(5));
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
