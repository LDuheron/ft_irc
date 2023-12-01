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

void	Command::handleJoin(Client *client, std::string message, std::map<std::string, Channel> _channels)
{
	std::string chanName = message.substr(6, message.size());
	std::string command = "JOIN " + chanName + "\r\n";
	// std::cout << "Channel name " << chanName << "\n";

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
		send(client->getSocket(), command.c_str(), command.size(), 0);
    }
}

void	Command::handlePing(int clientSocket, const std::string &pingData)
{
	std::string pongResponse = ":localhost PONG :" + pingData + "\n";
	if (send(clientSocket, pongResponse.c_str(), pongResponse.length(), 0) == FAIL)
		std::cerr << "Error : Failed to send pong.\n";
}

static void	handlePrivmsg()
{
	// target = nickname of a client or channel
	// if client is banned -> silently fail
	// <source> = of the message represents the user or server that sent the message. 
	// target starts with a $
	//  
	send("PRIVMSG" + _channels.first() + " :" + message + "\r\n");
	perror(404);
}
