
#include "Client.hpp"

// Constructor -----------------------------------------------------------------

Client::Client(int socket, Server *server) :
	_clientSocket(socket),
	_nickname("DEFAULT"),
	_username("DEFAULT"),
	_nickCheck(false),
	_passCheck(false),
	_event(),
	_server(server)
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

const std::string	&Client::getUsername(void) const { return (this->_username); }

void 				Client::setUsername(const std::string & username) { this->_username = username; }

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

Server				*Client::getServer(void) const { return (this->_server); }

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

int Client::isMemberOfChannel(std::string str)
{
	for (std::vector<Channel *>::const_iterator it = this->_isChannelMember.begin(); it != this->_isChannelMember.end(); it++)
	{
		if ((*it)->getName() == str)
			return (TRUE);
	}
	return (FALSE);
}

int Client::isBannedOfChannel(std::string str)
{
	for (std::vector<Channel *>::const_iterator it = this->_isBannedFromChannel.begin(); it != this->_isBannedFromChannel.end(); it++)
	{
		if ((*it)->getName() == str)
			return (TRUE);
	}
	return (FALSE);
}
