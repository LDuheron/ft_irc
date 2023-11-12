
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
