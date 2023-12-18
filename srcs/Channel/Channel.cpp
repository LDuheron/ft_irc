#include "Channel.hpp"
#include <string>
#include <vector>

# define MAX_USER 3

// Constructor -----------------------------------------------------------------

Channel::Channel(const string &name, Server *server) :
	_name(name),
	_members(),
	_operator(),
	_invited(),
	_hasPassword(FALSE),
	_inviteOnly(FALSE),
	_hasUserLimit(FALSE),
	_hasTopicProtection(FALSE),
	_topic(""),
	_server(server),
	_cptUser(0),
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

vector<Client *> const	&Channel::getInvited(void) const { return (this->_invited); }

const std::string &	Channel::getTopic(void) { return (this->_topic); }

void	Channel::setTopic(std::string const &topic) { this->_topic = topic; }

Server *Channel::getServer(void) const { return (this->_server); }


// Overload --------------------------------------------------------------------

// Functions -------------------------------------------------------------------

// MEMBER 

void	Channel::addMember(Client *client)
{
	this->_members.push_back(client);
	this->_cptUser += 1;
}

bool	Channel::isMember(Client *client)
{
	for (std::vector<Client*>::const_iterator itClient = this->_members.begin(); itClient != this->_members.end(); itClient++)
	{
		if ((*itClient)->getNickname() == client->getNickname())
			return (true);
	}
	return (false);
}

void	Channel::removeMember(Client *client)
{
	for (std::vector<Client*>::iterator itClient = this->_members.begin(); itClient != this->_members.end(); itClient++)
	{
		if ((*itClient)->getNickname() == client->getNickname())
		{
			this->_members.erase(itClient);
			this->_cptUser -= 1;
			break;
		}
	}
}

void	Channel::removeMember(std::string client)
{
	for (std::vector<Client*>::iterator itClient = this->_members.begin(); itClient != this->_members.end(); itClient++)
	{
		if ((*itClient)->getNickname() == client)
		{
			this->_members.erase(itClient);
			this->_cptUser -= 1;
			break ;
		}
	}
}

// Operators

void	Channel::addOperator(Client *client) { this->_operator.push_back(client); }

void	Channel::removeOperator(Client *client)
{
	for (std::vector<Client*>::iterator itOperator = this->_operator.begin(); itOperator != this->_operator.end(); itOperator++)
	{
		if ((*itOperator)->getNickname() == client->getNickname())
		{
			this->_operator.erase(itOperator);
			break ;
		}
	}
}

bool	Channel::isOperator(Client *client)
{
	for (std::vector<Client*>::const_iterator itOperator = this->_operator.begin(); itOperator != this->_operator.end(); itOperator++)
	{
		if ((*itOperator)->getNickname() == client->getNickname())
			return (true);
	}
	return (false);
}

// INVITE

void	Channel::setInviteOnly(bool inviteOnly) { this->_inviteOnly = inviteOnly; }

bool	Channel::getInviteOnly(void) const { return (this->_inviteOnly); }


void	Channel::inviteMember(Client *client)
{
	this->_invited.push_back(client);
}

void	Channel::uninviteMember(Client *client)
{
	for (std::vector<Client*>::iterator itInvited = this->_invited.begin(); itInvited != this->_invited.end(); itInvited++)
	{
		if ((*itInvited)->getNickname() == client->getNickname())
		{
			this->_invited.erase(itInvited);
			break ;
		}
	}
}

bool	Channel::isInvited(Client *client)
{
	for (std::vector<Client*>::const_iterator itInvited = this->_invited.begin(); itInvited != this->_invited.end(); itInvited++)
	{
		if ((*itInvited)->getNickname() == client->getNickname())
			return (true);
	}
	return (false);
}

void	Channel::listUsers()
{}

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

const	string	&Channel::getPassword(void) const { return (this->_password); }

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

size_t		Channel::getUserLimit(void) const { return (this->_userLimit); }


void Channel::setTopicProtection(bool hasTopicProtection) { this->_hasTopicProtection = hasTopicProtection; }

bool Channel::getTopicProtection(void) const { return (this->_hasTopicProtection); }
