#include "../Channel/Channel.hpp"
#include "../Server/Server.hpp"

# define MAX_USER 3

// Constructor -----------------------------------------------------------------

Channel::Channel() : _banned(), _members(), _operator(), _invited(),
_maxUser(MAX_USER), _hasPassword(FALSE), _password("NULL"),
_topic("NULL"), _type(REGULAR)
{
	(void) _hasPassword;
	(void) _type;
	if (DEBUG)
		std::cout << "Channel " << this->_topic << " default constructor called\n";
}

Channel::Channel(Channel const & src)
{
	*this = src;
	if (DEBUG)
		std::cout << "Channel " << this->_topic << " default constructor called\n";
}

// Destructor ------------------------------------------------------------------
Channel::~Channel()
{
	if (DEBUG)
		std::cout << "Channel " << this->_topic << " destructor called\n";
}

// Accessors -------------------------------------------------------------------

const std::string &	Channel::getTopic(void)
{
	return (this->_topic);
}

void	Channel::setTopic(std::string const &topic)
{
	this->_topic = topic;
}

const int &	Channel::getMaxUser(void)
{
	return (this->_maxUser);
}

void	Channel::setMaxUser(int const &maxUser)
{
	this->_maxUser = maxUser;
}

const int &	Channel::getType(void)
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
	(void)rhs;
	// for (int i = 0; i < (int)rhs._admin.size(); i++)
	// 	this->_admin.push_back(rhs._admin[i]);
	// for (int i = 0; i < (int)rhs._banned.size(); i++)
	// 	this->_banned.push_back(rhs._banned[i]);
	// for (int i = 0; i < (int)rhs._members.size(); i++)
	// 	this->_members.push_back(rhs._members[i]);
	// this->_topic = rhs._topic;
	// this->_nbUsers = rhs._nbUsers;
	// for (int i = 0; i < (int)rhs._operators.size(); i++)
	// 	this->_operators.push_back(rhs._operators[i]);
	// this->_password = rhs._password;
	// this->_topic = rhs._topic;
	// this->_type = rhs._type;
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
