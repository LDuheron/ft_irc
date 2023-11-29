
#include "Client.hpp"

// Constructor -----------------------------------------------------------------

Client::Client(int socket) :
	_clientSocket(socket),
	_nickname("DEFAULT"),
	_username("DEFAULT"),
	_nickCheck(false),
	_passCheck(false),
	_event()
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

void				Client::setSocket(int fd) { this->_clientSocket = fd; }

int					Client::getSocket(void) const { return (this->_clientSocket); }

const std::string	&Client::getNickname(void) const { return (this->_nickname); }

void 				Client::setNickname(const std::string & nickname) { this->_nickname = nickname; }

bool				Client::getNickCheck(void) const { return (this->_nickCheck); }

void				Client::setNickCheck(void) { this->_nickCheck = true; }

bool				Client::getPassCheck(void) const { return (this->_passCheck); }

void				Client::setPassCheck(void) { this->_passCheck = true; }

void				Client::setEvent()
{
	this->_event.events = EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLERR; // EPOLLHUP : hung up, EPOLLERR : error on the socket
	this->_event.data.fd = this->_clientSocket;
}

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
