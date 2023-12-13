
#include "Client.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>

// Constructor -----------------------------------------------------------------

Client::Client(int socket, Server *server) :
	_clientSocket(socket),
	_nickname("DEFAULT"),
	_username("DEFAULT"),
	_hostname("127.0.0.1"),
	_realname("DEFAULT"),
	_nickCheck(false),
	_userCheck(false),
	_passCheck(false),
	_event(),
	_server(server),
	_incomplete(false),
	_capLSsent(false),
	_askPassword(false),
	_isConnected(false)
{
	setEvent();
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

bool				Client::getCapLSsent(void) const { return (this->_capLSsent); }\

void				Client::setIncompleteMessage(const std::string & incompleteMessage) { this->_incompleteMessage = incompleteMessage; }

const std::string	&Client::getIncompleteMessage(void) const { return (this->_incompleteMessage); }


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

bool				Client::getUserCheck(void) const { return (this->_userCheck); }

void				Client::setUserCheck(void) { this->_userCheck = true; }

bool				Client::getPassCheck(void) const { return (this->_passCheck); }

void				Client::setPassCheck(void) { this->_passCheck = true; }

void				Client::setEvent()
{
	this->_event.events = EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLERR; // EPOLLHUP : hung up, EPOLLERR : error on the socket
	this->_event.data.fd = this->_clientSocket;
}

struct epoll_event	*Client::getEventAddress(void) { return (&this->_event); }

Server				*Client::getServer(void) const { return (this->_server); }

bool				Client::getAskPassword(void) const { return (this->_askPassword); }

void				Client::setAskPassword(bool askPassword) { this->_askPassword = askPassword; }

bool				Client::getIsConnected(void) const { return (this->_isConnected); }

void				Client::setIsConnected(bool isConnected) { this->_isConnected = isConnected; }

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

bool	Client::isOperator(Channel *channel)
{
	for (std::vector<Client *>::const_iterator it = channel->getOperators().begin(); it != channel->getOperators().end(); ++it)
	{
		if (*it == this)
			return (true);
	}
	return (false);
}
