#include "Channel.hpp"

# define MAX_USER 3

// Constructor -----------------------------------------------------------------

Channel::Channel() :
	_banned(),
	_members(),
	_operator(),
	_invited(),
	_hasPassword(FALSE),
	_maxUser(MAX_USER),
	_password("NULL"),
	_topic("NULL"),
	_type(REGULAR)
{}

Channel::Channel(Channel const & src)
{
	*this = src;
}

// Destructor ------------------------------------------------------------------
Channel::~Channel()
{}

// Accessors -------------------------------------------------------------------

// const std::string &	Channel::getPassword(void)
// {
// 	return (this->_password);
// }

const std::string &	Channel::getName(void) const
{
	return (this->_name);
}

void	Channel::setPassword(std::string const &password)
{
	this->_password = password;
}

const std::string &	Channel::getTopic(void) const
{
	return (this->_topic);
}

void	Channel::setTopic(std::string const &topic)
{
	this->_topic = topic;
}

const int &	Channel::getMaxUser(void) const
{
	return (this->_maxUser);
}

void	Channel::setMaxUser(int const &maxUser)
{
	this->_maxUser = maxUser;
}

const int &	Channel::getType(void) const
{
	return (this->_type);
}

void	Channel::setType(int const &type)
{
	this->_type = type;
}

// Overload --------------------------------------------------------------------

Channel &	Channel::operator=(Channel const & rhs)
{
	for (int i = 0; i < (int)rhs._banned.size(); i++)
		this->_banned.push_back(rhs._banned[i]);
	for (int i = 0; i < (int)rhs._members.size(); i++)
		this->_members.push_back(rhs._members[i]);
	for (int i = 0; i < (int)rhs._operator.size(); i++)
		this->_operator.push_back(rhs._operator[i]);
	for (int i = 0; i < (int)rhs._invited.size(); i++)
		this->_invited.push_back(rhs._invited[i]);
	this->_hasPassword = rhs._hasPassword;
	this->_maxUser = rhs._maxUser;
	this->_password = rhs._password;
	this->_topic = rhs._topic;
	this->_type = rhs._type;
	return (*this);
}

// Functions -------------------------------------------------------------------

void	Channel::banMember(Client *client)
{
	this->_banned.push_back(client);
	if (DEBUG)
		std::cout << "Ban client successfully\n";
}

void	Channel::unbanMember(Client *client)
{
	for (int i = 0; i < (int)this->_banned.size(); i++)
	{
		if (this->_banned[i] == client)
		{
			this->_banned.erase(_banned.begin() + i);
			std::cout << "Client succesfully banned.\n";
			break ;
		}
	}
}

void	Channel::addMember(Client *client)
{
	this->_members.push_back(client);
	if (DEBUG)
		std::cout << "Add client successfully\n";
}

void	Channel::removeMember(Client *client)
{
	for (int i = 0; i < (int)this->_members.size(); i++)
	{
		if (this->_members[i] == client)
		{
			this->_members.erase(_members.begin() + i);
			std::cout << "Client succesfully erased.\n";
			break ;
		}
	}
}

void	Channel::addOperator(Client *client)
{
	this->_members.push_back(client);
	if (DEBUG)
		std::cout << "Add client to operator successfully\n";
}

void	Channel::removeOperator(Client *client)
{
	for (int i = 0; i < (int)this->_operator.size(); i++)
	{
		if (this->_operator[i] == client)
		{
			this->_operator.erase(_operator.begin() + i);
			std::cout << "Suppressed client from operator.\n";
			break ;
		}
	}
}

void	Channel::inviteMember(Client *client)
{
	this->_members.push_back(client);
	if (DEBUG)
		std::cout << "Invite client successfully\n";
}

void	Channel::uninviteMember(Client *client)
{
	for (int i = 0; i < (int)this->_operator.size(); i++)
	{
		if (this->_operator[i] == client)
		{
			this->_operator.erase(_operator.begin() + i);
			std::cout << "Uninvited client succesfully.\n";
			break ;
		}
	}
}

//// is something

int	Channel::isBanned(Client *client)
{
	for (int i = 0; i < (int)this->_banned.size(); i++)
	{
		if (this->_banned[i] == client)
		{
			this->_banned.erase(_banned.begin() + i);
			std::cout << "Client is invited.\n";
			return (TRUE);
		}
	}
	return (FALSE);
}

int	Channel::isMember(Client *client)
{
	for (int i = 0; i < (int)this->_members.size(); i++)
	{
		if (this->_members[i] == client)
		{
			this->_members.erase(_members.begin() + i);
			std::cout << "Client is invited.\n";
			return (TRUE);
		}
	}
	return (FALSE);
}

int	Channel::isInvited(Client *client)
{
	for (int i = 0; i < (int)this->_operator.size(); i++)
	{
		if (this->_operator[i] == client)
		{
			this->_operator.erase(_operator.begin() + i);
			std::cout << "Client is invited.\n";
			return (TRUE);
		}
	}
	return (FALSE);
}

int	Channel::isOperator(Client *client)
{
	for (int i = 0; i < (int)this->_operator.size(); i++)
	{
		if (this->_operator[i] == client)
		{
			this->_operator.erase(_operator.begin() + i);
			std::cout << "Client is operator.\n";
			return (TRUE);
		}
	}
	return (FALSE);
}
