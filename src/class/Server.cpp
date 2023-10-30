
#include "Server.hpp"

// Constructor -----------------------------------------------------------------

Server::Server() :
	_IP(0), _password("NULL"),
	_port(0), _socket(0)
{
	if (DEBUG)
		std::cout << "Default constructor called.\n";
}

Server::Server(Server const & src) : 
	_IP(src._IP), _password(src._password),
	_port(src._port), _socket(src._socket)
{
	if (DEBUG)
		std::cout << "Copy constructor called.\n";
}

// Destructor ------------------------------------------------------------------

Server::~Server()
{
	if (DEBUG)
		std::cout << "Destructor called.\n";
}

// Accessors -------------------------------------------------------------------

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

// Overload --------------------------------------------------------------------

Server &	Server::operator=(Server const & rhs)
{
	this->_IP = rhs._IP;
	this->_password = rhs._password;
	this->_port = rhs._port;
	this->_socket = rhs._socket;
	return *this;
}

std::ostream & operator<<(std::ostream & lhs, Server const & rhs)
{
	lhs << "Password : " << rhs.getPassword()
		<< "\nIP : " << rhs.getIP()
		<< "\nPort : " << rhs.getPort()
		<< "\nSocket : Unavailable"
		<< ".\n";
	return lhs;
}

// Functions -------------------------------------------------------------------
