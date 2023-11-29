#include "Command.hpp"
#include <cstdio>
#include <string>
#include <vector>

using std::string;
using std::vector;

// Constructor -----------------------------------------------------------------

Command::Command(Server *server)
	// _server(server)
{
	(void)server;
	registerCommand("CAP LS", sendCAPLs);
	registerCommand("PASS", sendPASSMessage);
	// registerCommand("JOIN", handleJoin);
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

void						Command::registerCommand(const std::string &command, CommandFunction function)
{
	this->_commandMap[command] = function;
}

std::vector<std::string>	Command::parseCommand(const std::string &command)
{
	vector<string>		parsedCommand;
	std::istringstream	iss(command);

		string				token;

		parsedCommand.push_back(token);
		while (iss >> token)
			parsedCommand.push_back(token);
		if (parsedCommand[0] == "CAP")
		{
			parsedCommand[0] += " " + parsedCommand[1];
			parsedCommand.erase(parsedCommand.begin() + 1);
		}

		std::cout << "===== parsed command : =====\n";
		for (std::vector<std::string>::iterator it = parsedCommand.begin(); it != parsedCommand.end(); ++it)
			std::cout << *it << "\n";
		std::cout << "============================\n";
	return (parsedCommand);
}

void						Command::handleCommand(const std::vector<std::string> &parsedCommand)
{
	if (!parsedCommand.empty())
	{
		const std::string &command = parsedCommand[0];
		std::map<std::string, CommandFunction>::iterator it = this->_commandMap.find(command);
		if (it != this->_commandMap.end())
			it->second(parsedCommand);
		else
			std::cerr <<"Error: Unknown command\n";
	}
}

void						Command::sendCAPLs(Client *client)
{
	std::string capLs = ":DEFAULT CAP * LS :none\n";
	if (send(client->getSocket(), capLs.c_str(), capLs.length(), MSG_NOSIGNAL) == -1)
		std::perror("Error : Failed to send CAP LS\n");
}

void						Command::sendRPLMessages(Client *client)
{
	std::string welcome1 = ":DEFAULT 001 lletourn :Welcome to the Internet Relay Network lletourn!user@host\n";
	std::string welcome2 = ":DEFAULT 002 lletourn :Your host is DEFAULT, running version 0.1\n";
	std::string welcome3 = ":DEFAULT 003 lletourn :This server was created 2023/11/20\n";
	std::string welcome4 = ":DEFAULT 004 lletourn DEFAULT ircd_version user_modes chan_modes\n";

		if (send(client->getSocket(), welcome1.c_str(), welcome1.length(), MSG_NOSIGNAL) == -1)
			std::perror("Error : Failed to send 001.\n");
		if (send(client->getSocket(), welcome2.c_str(), welcome2.length(), MSG_NOSIGNAL) == -1)
			std::perror("Error : Failed to send 002.\n");
		if (send(client->getSocket(), welcome3.c_str(), welcome3.length(), MSG_NOSIGNAL) == -1)
			std::perror("Error : Failed to send 003.\n");
		if (send(client->getSocket(), welcome4.c_str(), welcome4.length(), MSG_NOSIGNAL) == -1)
			std::perror("Error : Failed to send 004.\n");
}

void						Command::sendPASSMessage(Client *client)
{
	std::string pass = "Password required. Try /quote PASS <password>\r\n\r\n";
	if (send(client->getSocket(), pass.c_str(), pass.length(), MSG_NOSIGNAL) == -1)
		std::perror("Error : Failed to send PASS message\n");
}

void						Command::checkPassword(Client *client, const std::string &password, const std::string &givenPassword)
{
	string	passError = ":localhost 464 * :Password incorrect\n";
	string	passOk = ":localhost 001 * :Welcome to the Internet Relay Network\n";

	if (client->getPassCheck() == false && password == givenPassword)
	{
		client->setPassCheck();
		sendRPLMessages(client);
	}
	else
		if (send(client->getSocket(), passError.c_str(), passError.length(), MSG_NOSIGNAL) == -1)
			std::perror("Error : Failed to send password error message\n");
}

void						Command::handleJoin(Client *client, std::string message, std::map<std::string, Channel> _channels)
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
