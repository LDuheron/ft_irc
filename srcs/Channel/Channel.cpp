#include "Channel.hpp"
#include <vector>

# define MAX_USER 3

// Constructor -----------------------------------------------------------------

Channel::Channel(const string &name) :
	_name(name),
	_banned(),
	_members(),
	_operator(),
	_invited(),
	_hasPassword(FALSE),
	_topic(NULL),
	_password(NULL),
	_userLimit(MAX_USER)
{}

// Channel::Channel(Channel const & src)
// {
// 	*this = src;
// }

// Destructor ------------------------------------------------------------------
Channel::~Channel()
{}

// Accessors -------------------------------------------------------------------

// const std::string &	Channel::getPassword(void)
// {
// 	return (this->_password);
// }

const std::string &	Channel::getName(void) const { return (this->_name); }

// void	Channel::setName(std::string const &name) { this->_name = name; }

vector<Client *> const	&Channel::getMembers(void) const { return (this->_members); }

vector<Client *> const	&Channel::getOperators(void) const { return (this->_operator); }

const std::string &	Channel::getTopic(void) { return (this->_topic); }

void	Channel::setTopic(std::string const &topic) { this->_topic = topic; }

// const int &	Channel::getMaxUser(void) { return (this->_maxUser); }

// void	Channel::setMaxUser(int const &maxUser) { this->_maxUser = maxUser; }

// Overload --------------------------------------------------------------------

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

void	Channel::addMember(Client *client) { this->_members.push_back(client); }

void	Channel::removeMember(Client *client)
{
	for (size_t i = 0; i < this->_members.size(); i++)
	{
		if (this->_members[i] == client)
		{
			this->_members.erase(_members.begin() + i);
			break ;
		}
	}
}

void	Channel::addOperator(Client *client) { this->_operator.push_back(client); }

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
			this->_operator.erase(_operator.begin() + i); // a modifier
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
			this->_operator.erase(_operator.begin() + i); // a modifier
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
			this->_operator.erase(_operator.begin() + i); // a modifier
			std::cout << "Client is operator.\n";
			return (TRUE);
		}
	}
	return (FALSE);
}

void	Channel::listUsers()
{}

bool	Channel::isMember(Client *client)
{
	for (size_t i = 0; i < this->_members.size(); i++)
	{
		if (this->_members[i] == client)
		{
			this->_members.erase(_members.begin() + i);
			return (1);
		}
	}
	return (0);
}

void	Channel::setMode(char mode) { this->_modes.insert(mode); }

void	Channel::unsetMode(char mode) { this->_modes.erase(mode); }

bool	Channel::isModeSet(char mode) const
{
	if (this->_modes.find(mode) != this->_modes.end())
		return (true);
	return (false);
}

void	Channel::setPassword(const std::string &password) { this->_password = password; }

void	Channel::unsetPassword(void) { this->_password = "NULL"; }

bool	Channel::checkPassword(const string &password) const
{
	if (password == this->_password)
		return (true);
	return (false);
}

void	Channel::setUserLimit(int limit) { this->_userLimit = limit; }

void	Channel::unsetUserLimit(void) { this->_userLimit = -1; }

int		Channel::getUserLimit(void) const { return (this->_userLimit); }
