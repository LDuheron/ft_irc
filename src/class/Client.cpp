
#include "../../private/Client.hpp"

// Constructor -----------------------------------------------------------------

Client::Client() : _fd(), _nickname("DEFAULT"), _username("DEFAULT")
{
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

void	Client::setFd(int fd)
{
	this->_fd = fd;
}

int		Client::getFd(void) const
{
	return (this->_fd);
}

// Overload --------------------------------------------------------------------

Client &	Client::operator=(Client const & rhs)
{
	this->_fd = rhs._fd;
	this->_nickname = rhs._nickname;
	return (*this);
}


// Functions -------------------------------------------------------------------

void	Client::handlePing(const std::string &pingData)
{
	std::string pongResponse = "PONG :" + pingData + "\r\n";

	if (send(this->_fd, pongResponse.c_str(), pongResponse.length(), 0) == FAIL)
		std::cerr << "Error: Failed to send PONG response.\n";
}
