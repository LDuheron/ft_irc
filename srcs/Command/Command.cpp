#include "Command.hpp"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

// Constructor -----------------------------------------------------------------

Command::Command()
{
	registerCommand("CAP LS", sendCAPLs);
	registerCommand("CAP END", doNothing);
	registerCommand("PASS", checkPassword);
	registerCommand("PING", handlePing);
	registerCommand("NICK", handleNick);
	registerCommand("USER", handleUser);

	registerCommand("SHUTDOWN", shutdown);

	registerCommand("QUIT", doNothing);
	registerCommand("WHOIS", doNothing);
	registerCommand("MODE", doNothing);

	// registerCommand("JOIN", handleJoin);
}

Command::~Command()
{
	this->_commandMap.clear();
}

// Destructor ------------------------------------------------------------------

// Accessors -------------------------------------------------------------------

// Overload --------------------------------------------------------------------

// Functions -------------------------------------------------------------------

void			Command::registerCommand(const std::string &command, CommandFunction function)
{
	this->_commandMap[command] = function;
}

vector<string> splitString(Client *client, const string &command, const string &delimiter)
{
	vector<string> result;

	string commandCopy = command;

	if (client->getIncomplete())
		commandCopy = client->getIncompleteMessage() + command;

	size_t	pos = 0;
	size_t	end = commandCopy.find(delimiter);

	while (end != std::string::npos)
	{
		result.push_back(commandCopy.substr(pos, end - pos));
		pos = end + delimiter.size();
		end = commandCopy.find(delimiter, pos);
	}
	if (pos < commandCopy.size())
	{
		client->setIncomplete(true);
		client->setIncompleteMessage(commandCopy.substr(pos, end - pos));
	}
	else
		client->setIncomplete(false);
	return (result);
}

void		Command::exec(Client *client, vector<string> &parsedCommand)
{
	if (parsedCommand.empty())
		return;
	std::map<string, CommandFunction>::iterator it = this->_commandMap.find(parsedCommand[0]);
	if (it == this->_commandMap.end())
	{
		std::cerr <<"Error: Unknown command: " << parsedCommand[0] << "\n";
		return;
	}
	if (client->getPassCheck() == false && parsedCommand[0] != "PASS" && parsedCommand[0] != "CAP LS" && parsedCommand[0] != "CAP END")
	{
		if (parsedCommand[0] != "NICK" && parsedCommand[0] != "USER")
		{
			std::string passError = ":" + client->getServer()->getNickname() + " 451 * :You have not registered (cmd:\n" + parsedCommand[0] + ")\n";
			if (send(client->getSocket(), passError.c_str(), passError.length(), MSG_NOSIGNAL) == -1)
				std::perror("Error : Failed to send password error message");
		}
		if (!client->getAskPassword())
		{
			Command::sendPASSMessage(client);
			client->setAskPassword(true);
		}
		if ((client->getNickCheck() == false && parsedCommand[0] == "NICK")
			||( client->getUserCheck() == false && parsedCommand[0] == "USER"))
		{
			this->_commandBuffer.push_back(parsedCommand);
			parsedCommand.clear();
		}
	}
	else if (!parsedCommand.empty())
	{
		this->_commandMap[parsedCommand[0]](client, parsedCommand);
		if (client->getPassCheck())
		{
			while (!this->_commandBuffer.empty())
			{
				this->_commandMap[this->_commandBuffer[0][0]](client, this->_commandBuffer[0]);
				this->_commandBuffer.erase(this->_commandBuffer.begin());
			}
		}
	}
}

void	Command::execCmds(Client *client, const std::string &command)
{
	vector<string>			line = splitString(client, command, DELIMITER);
	while (!line.empty())
	{
		std::istringstream	iss(line[0]);
		std::string			token;
		vector<string>		parsedCommand;

		while (iss >> token)
			parsedCommand.push_back(token);
		if (!parsedCommand.empty() && parsedCommand[0] == "CAP")
		{
			parsedCommand[1] = "CAP " + parsedCommand[1];
			parsedCommand.erase(parsedCommand.begin());
		}
		std::cout << "\n===== Data received from client : =====\n";
		for (std::vector<std::string>::iterator it2 = parsedCommand.begin(); it2 != parsedCommand.end(); ++it2)
			std::cout << *it2 << "|";
		std::cout << "\n=======================================\n\n";
		exec(client, parsedCommand);
		line.erase(line.begin());
	}
}

void	Command::sendCAPLs(Client *client, vector<string> &parsedCommand)
{
	(void)parsedCommand;

		std::string capLs = ":DEFAULT CAP * LS :none\n";
		if (send(client->getSocket(), capLs.c_str(), capLs.length(), MSG_NOSIGNAL) == -1)
			std::perror("Error : Failed to send CAP LS");
}

void	Command::sendRPLMessages(Client *client, vector<string> &parsedCommand)
{
	(void)parsedCommand;
	std::string welcome1 = ":" + client->getServer()->getNickname() + " 001 " + client->getNickname() + ":Welcome to the Internet Relay Network " + client->getNickname() + "!" + client->getUsername() + "@" + client->getServer()->getNickname() + "\n";
	std::string welcome2 = ":" + client->getServer()->getNickname() + " 002 " + client->getNickname() + ":Your host is " + client->getServer()->getNickname()+ ", running version 0.1\n";
	std::string welcome3 = ":" + client->getServer()->getNickname() + " 003 " + client->getNickname() + ":This server was created 2023/11/20\n";
	std::string welcome4 = ":" + client->getServer()->getNickname() + " 004 " + client->getNickname() + ":" + client->getServer()->getNickname() + " ircd_version user_modes chan_modes\n";

	Server::sendMessage(client, welcome1);
	Server::sendMessage(client, welcome2);
	Server::sendMessage(client, welcome3);
	Server::sendMessage(client, welcome4);

	client->setIsConnected(true);
}

void	Command::sendPASSMessage(Client *client)
{
	if (client->getPassCheck() == true)
		return;
	Command::sendNewLine(client);
	std::string pass = "Password required.\nTry /quote PASS <password>\n";
	Server::sendMessage(client, pass);
	Command::sendNewLine(client);
}

void	Command::checkPassword(Client *client, vector<string> &parsedCommand)
{
	string	passError = ":" + client->getServer()->getNickname() + " 464 * :Password incorrect\n";

	if (client->getPassCheck() == false && parsedCommand[1] == client->getServer()->getPassword())
		client->setPassCheck();
	else
		Server::sendMessage(client, passError);
}

void	Command::handlePing(Client *client, vector<string> &parsedCommand)
{
	std::string pingData;

	pingData += parsedCommand[1] + " ";
	std::string pongResponse = ":" + client->getServer()->getNickname() + " PONG :" + pingData + "\n";
	Server::sendMessage(client, pongResponse);
}

static void	handleValidNick(Client *client, vector<string> &parsedCommand)
{
	client->setNickname(parsedCommand[1]);
	client->setNickCheck();
	if (client->getUserCheck() && !client->getIsConnected())
		Command::sendRPLMessages(client, parsedCommand);
}

static bool	checkValidNick(Client *client, const string &nickname)
{
	string nickError = ":" + client->getServer()->getNickname() + " 432 * " + nickname + " :Erroneus nickname\n";
	bool res = true;

	if (nickname == "anonymous")
		res = false;
	else if (nickname.length() > 9)
		res = false;
	else
	{
		for (size_t i = 0; i < nickname.length(); ++i)
		{
			if (nickname[i] == ' ')
				res = false;
		}
	}
	if (!res)
		Server::sendMessage(client, nickError);
	return (res);
}

static bool	checkNickInUse(Client *client, const string &nickname)
{
	string nickError = ":" + client->getServer()->getNickname() + " 433 * " + nickname + " :Nickname is already in use\n";

	for (std::map<int, Client *>::const_iterator it = client->getServer()->getClientMap().begin(); it != client->getServer()->getClientMap().end(); ++it)
	{
		if (it->second->getNickname() == nickname)
		{
			Server::sendMessage(client, nickError);
			return (true);
		}
	}
	return (false);
}

// ERR_NONICKNAMEGIVEN 			OK
// ERR_ERRONEUSNICKNAME			OK
// ERR_NICKNAMEINUSE			OK
// ERR_NICKCOLLISION			FLEMME
// ERR_UNAVAILRESOURCE			FLEMME
// ERR_RESTRICTED				X
void	Command::handleNick(Client *client, vector<string> &parsedCommand)
{
	if (parsedCommand.size() < 2)
	{
		string nickError = ":" + client->getServer()->getNickname() + " 431 * :No nickname given\n";
		Server::sendMessage(client, nickError);
		return;
	}
	if (parsedCommand.size() > 2)
	{
		string nickError = ":" + client->getServer()->getNickname() + " 432 * " + parsedCommand[1] + " :Erroneus nickname\n";
		Server::sendMessage(client, nickError);
		return;
	}
	if (!checkValidNick(client, parsedCommand[1]))
		return;
	if (checkNickInUse(client, parsedCommand[1]))
		return;
	handleValidNick(client, parsedCommand);
}

// ERR_NEEDMOREPARAMS			OK
// ERR_ALREADYREGISTRED			OK
void	Command::handleUser(Client *client, vector<string> &parsedCommand)
{
	if (parsedCommand.size() < 5)
	{
		std::string userError = ":" + client->getServer()->getNickname() + " 461 * USER :Not enough parameters\n";
		if (send(client->getSocket(), userError.c_str(), userError.length(), MSG_NOSIGNAL) == -1)
			std::perror("Error : Failed to send user error message");
		return;
	}
	else if (client->getUserCheck())
	{
		std::string AlreadyRegistered = ":" + client->getServer()->getNickname() + " 462 * :Unauthorized command (already registered)\n";
		if (send(client->getSocket(), AlreadyRegistered.c_str(), AlreadyRegistered.length(), MSG_NOSIGNAL) == -1)
			std::perror("Error : Failed to send already registered error message");
		return;
	}
	else
	{
			client->setUsername(parsedCommand[1]);
			client->setUserCheck();
			if (client->getNickCheck() && !client->getIsConnected())
				Command::sendRPLMessages(client, parsedCommand);
	}
}

void	Command::doNothing(Client *Client, vector<string> &cmd)
{
	(void)Client;
	(void)cmd;
}

void	Command::shutdown(Client *client, vector<string> &cmd)
{
	(void)cmd;
	client->getServer()->setShutdown(true);
}

void	Command::sendNewLine(Client *client)
{
	std::string newLine = " \n";
	if (send(client->getSocket(), newLine.c_str(), newLine.length(), MSG_NOSIGNAL) == -1)
		std::perror("Error : Failed to send new line");
}

// vector<string>						Command::handleJoin(Client *client, std::string message, std::map<std::string, Channel> _channels)
// {
// 	std::string chanName = message.substr(6, message.size());
// 	std::cout << "Channel name " << chanName << "\n";

// 	std::map<std::string, Channel>::iterator it = _channels.find(chanName);
// 	if (it != _channels.end())
// 	{
// 		it->second.addMember(client);
// 		it->second.addOperator(client);
// 	}
// 	else
// 	{
// 		Channel	newChannel;

// 		if (message[6] == '&')
// 			newChannel.setType(LOCAL);
// 		newChannel.addMember(client);
// 		newChannel.addOperator(client);
// 		_channels.insert(std::make_pair(chanName, newChannel));
// 	}
// }
