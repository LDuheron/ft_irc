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

// add operators

void	Channel::addMember(Client *client)
{
	this->_members.push_back(client);
	if (DEBUG)
		std::cout << "Add client successfully\n";
}

// void	Channel::removeMember(Client *client)
// {
// 	for (int i = 0; i < this->_members.size(); i++)
// 	{
// 		if (this->_members[i] == client)
// 		{
// 			this->_members.erase(i);
// 			std::cout << "Client succesfully erased.\n";
// 			break ;
// 		}
// 	}
// }


// addmembers
// remove user
// set max SI_USER
// is invites/

// set user mode 
// bann