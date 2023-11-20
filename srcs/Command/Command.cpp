#include "../Command/Command.hpp"

// Constructor -----------------------------------------------------------------

Command::Command()
{
	if (DEBUG)
		std::cout << "Command default constructor called.";
}

Command::Command(Command const & src)
{
	*this = src;
	if (DEBUG)
		std::cout << "Command copy constructor called.";
}

// Destructor ------------------------------------------------------------------

Command::~Command()
{
	if (DEBUG)
		std::cout << "Command destructor called.";
}

// Accessors -------------------------------------------------------------------

// Overload --------------------------------------------------------------------

Command &	Command::operator=(Command const & rhs)
{
	(void)rhs;
	return (*this);
}

// Functions -------------------------------------------------------------------

void	Command::join(Client *client, std::string message, std::map<std::string, Channel> _channels)
{
	std::string chanName = message.substr(6, message.size());
	std::cout << "Channel name " << chanName << "\n";

    std::map<std::string, Channel>::iterator it = _channels.find(chanName);
    if (it != _channels.end())
	{
        it->second.addMember(client);
        it->second.addOperator(client);
    }
	else 
	{
        Channel	newChannel;

		if (message[6] == '&')
			newChannel.setType(LOCAL);
		newChannel.addMember(client);
		newChannel.addOperator(client);
		_channels.insert(std::make_pair(chanName, newChannel));
    }
}
