
#include "Server.hpp"

// Constructor -----------------------------------------------------------------

Server::Server() :
	_IP(0), _password("NULL"),
	_port(0), _socket(0),
	_nickname("DEFAULT")
{
	if (DEBUG)
		std::cout << "Server : default constructor called.\n";
}

Server::Server(int port, std::string password) :
	_IP(0), _password(password),
	_port(port), _socket(0),
	_nickname("DEFAULT")
{
	if (DEBUG)
		std::cout << "Server : Param port and password constructor called.\n";
}

Server::Server(Server const & src) : 
	_IP(src._IP), _password(src._password),
	_port(src._port), _socket(src._socket),
	_nickname(src._nickname)
{
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

// Overload --------------------------------------------------------------------

Server &	Server::operator=(Server const & rhs)
{
	this->_IP = rhs._IP;
	this->_nickname = rhs._nickname;
	this->_password = rhs._password;
	this->_port = rhs._port;
	this->_socket = rhs._socket;
	this->_serverAddr = rhs._serverAddr;
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

// Functions -------------------------------------------------------------------

// https://ncona.com/2019/04/building-a-simple-server-with-cpp/

// INIT_SERVERADDR : This function initializes the structure of data sockaddr_in.
// sin_family is AF_INET = gives to socket an IPv4 socket address to allow it to
// communicate with other hosts over a TCP/IP network.
// sin_port member defines the TCP/IP port number for the socket address.
void	Server::init_serverAddr(void)
{
	// htonl
	// this->_serverAddr.sin_addr = inet_addr("0.0.0.0"); // replace with actual server IP
	this->_serverAddr.sin_family = AF_INET;
	this->_serverAddr.sin_port = htons(this->_port);
	// this->_serverAddr.sin_zero =
}

// socket = file descritpor used for communication.
// AF_INET = IPV4
// SOCK_STREAM : Provides sequenced, reliable, bidirectional, connection-mode byte 
// streams, and may provide a transmission mechanism for out-of-band data.
// speifies that the commuinication is a two way reliale communicaction (TCP)
// only one protocol alvailable for each type so 0.

// BIND : used to assing an IP adress and port to the socket.

void	Server::init_server(void)
{
	// Init server addr structure
	init_serverAddr();
	setSocket(socket(AF_INET, SOCK_STREAM, 0));
	if (this->_socket == -1)
		std::cerr << "Error : Failed to create socket.\n";

	if (bind(this->_socket, (sockaddr*)&(this->_serverAddr), sizeof(this->_serverAddr)) == - 1)
		std::cerr << "Error : Failed to bind\n";

	// listen
	// RPL_WELCOME
}

// void	Server::loop(void)
// {
	// check inactivity : envoyer un ping ?
	// handle new client
	// handle new request
// }

// void	Server::check_inactivity()
// {
// 	send() ?????????????
// }


// void	Server::handleNewClient()
// {
// 	accept()
// }

// void	Server::handleNewRequest()
// {
	// recv ???????????????
	// bind ???(listen() "completes" bind)
// 	listen()
// accept()
// }
