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

// BAN

void	Channel::banMember(Client *client)
{
	this->_banned.push_back(client);
}

void	Channel::unbanMember(Client *client)
{
	for (std::vector<Client*>::const_iterator itBan = this->_banned.begin(); itBan != this->_banned.end(); itBan++)
	{
		if ((*itBan)->getNickname() == client->getNickname())
		{
			this->_banned.erase(itBan);
			std::cout << "Client succesfully unbanned.\n";
			break ;
		}
	}
}

bool	Channel::isBan(Client *client)
{
	for (std::vector<Client*>::const_iterator itBan = this->_banned.begin(); itBan != this->_banned.end(); itBan++)
	{
		if ((*itBan)->getNickname() == client->getNickname())
			return (true);
	}
	return (false);
}

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
	for (std::vector<Client*>::const_iterator itClient = this->_members.begin(); itClient != this->_members.end(); itClient++)
	{
		if ((*itClient)->getNickname() == client->getNickname())
		{
			this->_members.erase(itClient);
			this->_cptUser -= 1;
			std::cout << "Client succesfully remove.\n";
			break;
		}
	}
}

void	Channel::removeMember(std::string client)
{
	for (std::vector<Client*>::const_iterator itClient = this->_members.begin(); itClient != this->_members.end(); itClient++)
	{
		if ((*itClient)->getNickname() == client)
		{
			this->_members.erase(itClient);
			this->_cptUser -= 1;
			std::cout << "Client succesfully erased.\n";
			break ;
		}
	}
}

// Operators

void	Channel::addOperator(Client *client) { this->_operator.push_back(client); }

void	Channel::removeOperator(Client *client)
{
	for (std::vector<Client*>::const_iterator itOperator = this->_operator.begin(); itOperator != this->_operator.end(); itOperator++)
	{
		if ((*itOperator)->getNickname() == client->getNickname())
		{
			this->_operator.erase(itOperator);
			std::cout << "Operator succesfully remove.\n";
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
	this->_members.push_back(client);
	if (DEBUG)
		std::cout << "Invite client successfully\n";
}

void	Channel::uninviteMember(Client *client)
{

	for (std::vector<Client*>::const_iterator itInvited = this->_invited.begin(); itInvited != this->_invited.end(); itInvited++)
	{
		if ((*itInvited)->getNickname() == client->getNickname())
		{
			this->_invited.erase(itInvited);
			std::cout << "Client succesfully uninvited.\n";
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
