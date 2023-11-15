
#include "Client.hpp"
#include <sys/epoll.h>

// Constructor -----------------------------------------------------------------

Client::Client(int socket) : _clientSocket(socket), _nickname("DEFAULT"), _username("DEFAULT"), _event()
{
	setEvent();
	if (DEBUG)
		std::cout << "Client : default constructor called.\n";
}

// Destructor ------------------------------------------------------------------

Client::~Client(void)
{
	if (DEBUG)
		std::cout << "Client : destructor called.\n";
}
// Accessors -------------------------------------------------------------------

void				Client::setSocket(int fd)
{
	this->_clientSocket = fd;
}

int					Client::getSocket(void) const
{
	return (this->_clientSocket);
}

void				Client::setEvent()
{
	this->_event.events = EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLERR; // EPOLLHUP : hung up, EPOLLERR : error on the socket
	this->_event.data.fd = this->_clientSocket;
}

// struct epoll_event	Client::getEvent(void) const
// {
// 	return (this->_event);
// }

struct epoll_event	*Client::getEventAddress(void)
{
	return (&this->_event);
}

// Overload --------------------------------------------------------------------

Client &	Client::operator=(Client const & rhs)
{
	this->_clientSocket = rhs._clientSocket;
	this->_nickname = rhs._nickname;
	this->_username = rhs._username;
	this->_event = rhs._event;
	return (*this);
}


// Functions -------------------------------------------------------------------
