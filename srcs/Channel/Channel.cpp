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
	_cptUser(0),
	_maxUser(MAX_USER),
	_password("NULL"),
	_topic("NULL")
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

vector<Client *> const	&Channel::getInvited(void) const { return (this->_invited); }

void	Channel::setPassword(std::string const &password) { this->_password = password; }

const std::string &	Channel::getTopic(void) { return (this->_topic); }

void	Channel::setTopic(std::string const &topic) { this->_topic = topic; }

const int &	Channel::getMaxUser(void) { return (this->_maxUser); }

void	Channel::setMaxUser(int const &maxUser) { this->_maxUser = maxUser; }

// Overload --------------------------------------------------------------------

// Channel &	Channel::operator=(Channel const & rhs)
// {
// 	for (int i = 0; i < (int)rhs._banned.size(); i++)
// 		this->_banned.push_back(rhs._banned[i]);
// 	for (int i = 0; i < (int)rhs._members.size(); i++)
// 		this->_members.push_back(rhs._members[i]);
// 	for (int i = 0; i < (int)rhs._operator.size(); i++)
// 		this->_operator.push_back(rhs._operator[i]);
// 	for (int i = 0; i < (int)rhs._invited.size(); i++)
// 		this->_invited.push_back(rhs._invited[i]);
// 	this->_hasPassword = rhs._hasPassword;
// 	this->_maxUser = rhs._maxUser;
// 	this->_password = rhs._password;
// 	this->_topic = rhs._topic;
// 	return (*this);
// }

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
