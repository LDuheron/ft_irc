
#include "../../private/Client.hpp"

// Constructor -----------------------------------------------------------------

Client::Client(int id) : _fd(-1), _nickname("DEFAULT"), _username("DEFAULT"), _id(id), _event()
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

int		Client::getId(void) const
{
	return (this->_id);
}

// Overload --------------------------------------------------------------------

Client &	Client::operator=(Client const & rhs)
{
	this->_fd = rhs._fd;
	this->_nickname = rhs._nickname;
	this->_id = rhs._id;
	return (*this);
}


// Functions -------------------------------------------------------------------

void	Client::setEvent(int fd)
{
	this->_event.data.fd = fd;
	this->_event.events = EPOLLIN | EPOLLOUT;
}
