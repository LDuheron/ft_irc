#include "Channel.hpp"
#include <vector>

# define MAX_USER 3

// Constructor -----------------------------------------------------------------

Channel::Channel(const string &name, Server *server) :
	_name(name),
	_banned(),
	_members(),
	_operator(),
	_invited(),
	_hasPassword(FALSE),
	_inviteOnly(FALSE),
	_topic(""),
	_server(server),
	_password(""),
	_userLimit(MAX_USER)
{}

// Destructor ------------------------------------------------------------------
Channel::~Channel()
{}

// Accessors -------------------------------------------------------------------


const std::string &	Channel::getName(void) const { return (this->_name); }


vector<Client *> const	&Channel::getMembers(void) const { return (this->_members); }

vector<Client *> const	&Channel::getOperators(void) const { return (this->_operator); }

const std::string &	Channel::getTopic(void) { return (this->_topic); }

void	Channel::setTopic(std::string const &topic) { this->_topic = topic; }

Server *Channel::getServer(void) const { return (this->_server); }


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

void	Channel::setInviteOnly(bool inviteOnly) { this->_inviteOnly = inviteOnly; }

bool	Channel::getInviteOnly(void) const { return (this->_inviteOnly); }


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

std::set<char>	&Channel::getModes(void) { return (this->_modes); }

void	Channel::addMode(char mode) { this->_modes.insert(mode); }

void	Channel::removeMode(char mode) { this->_modes.erase(mode); }

std::string	Channel::getModesString(void)
{
	std::string modes;

	for (std::set<char>::iterator it = this->_modes.begin(); it != this->_modes.end(); ++it)
		modes += *it;
	return (modes);
}

void	Channel::setHasPassword(bool hasPassword) { this->_hasPassword = hasPassword; }

bool	Channel::getHasPassword(void) const { return (this->_hasPassword); }

void	Channel::setPassword(const std::string &password) { this->_password = password; }

bool	Channel::checkPassword(const string &password) const
{
	if (password == this->_password)
		return (true);
	return (false);
}

void	Channel::setHasUserLimit(bool hasUserLimit) { this->_hasUserLimit = hasUserLimit; }

bool	Channel::getHasUserLimit(void) const { return (this->_hasUserLimit); }

void	Channel::setUserLimit(int limit) { this->_userLimit = limit; }

int		Channel::getUserLimit(void) const { return (this->_userLimit); }


void Channel::setTopicProtection(bool hasTopicProtection) { this->_hasTopicProtection = hasTopicProtection; }

bool Channel::getTopicProtection(void) const { return (this->_hasTopicProtection); }
