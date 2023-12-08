
#include "Client.hpp"

// Constructor -----------------------------------------------------------------

Client::Client(int socket, Server *server) :
	_clientSocket(socket),
	_nickname("DEFAULT"),
	_username("DEFAULT"),
	_hostname("DEFAULT"),
	_realname("DEFAULT"),
	_nickCheck(false),
	_passCheck(false),
	_event(),
	_server(server),
	_incomplete(false),
	_capLSsent(false)
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

void				Client::setIncomplete(bool incomplete) { this->_incomplete = incomplete; }

bool				Client::getIncomplete(void) const { return (this->_incomplete); }

void				Client::setCapLSsent(bool capLSsent) { this->_capLSsent = capLSsent; }

bool				Client::getCapLSsent(void) const { return (this->_capLSsent); }



void				Client::setSocket(int fd) { this->_clientSocket = fd; }

int					Client::getSocket(void) const { return (this->_clientSocket); }

const std::string	&Client::getNickname(void) const { return (this->_nickname); }

void 				Client::setNickname(const std::string & nickname) { this->_nickname = nickname; }

const std::string	&Client::getUsername(void) const { return (this->_username); }

void 				Client::setUsername(const std::string & username) { this->_username = username; }

const std::string	&Client::getHostname(void) const { return (this->_hostname); }

void 				Client::setHostname(const std::string & hostname) { this->_hostname = hostname; }

const std::string	&Client::getRealname(void) const { return (this->_realname); }

void 				Client::setRealname(const std::string & realname) { this->_realname = realname; }

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
