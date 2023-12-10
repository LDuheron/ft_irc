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
	std::string welcome1 = ":" + client->getServer()->getNickname() + " 001 " + client->getNickname() + " :Welcome to the Internet Relay Network " + client->getNickname() + "!" + client->getUsername() + "@" + client->getServer()->getNickname() + "\n";
	std::string welcome2 = ":" + client->getServer()->getNickname() + " 002 " + client->getNickname() + ":Your host is " + client->getServer()->getNickname()+ ", running version 0.1\n";
	std::string welcome3 = ":" + client->getServer()->getNickname() + " 003 " + client->getNickname() + ":This server was created 2023/11/20\n";
	std::string welcome4 = ":" + client->getServer()->getNickname() + " 004 " + client->getNickname() + client->getServer()->getNickname() + " ircd_version user_modes chan_modes\n";

		if (send(client->getSocket(), welcome1.c_str(), welcome1.length(), MSG_NOSIGNAL) == -1)
			std::perror("Error : Failed to send 001.");
		if (send(client->getSocket(), welcome2.c_str(), welcome2.length(), MSG_NOSIGNAL) == -1)
			std::perror("Error : Failed to send 002.");
		if (send(client->getSocket(), welcome3.c_str(), welcome3.length(), MSG_NOSIGNAL) == -1)
			std::perror("Error : Failed to send 003.");
		if (send(client->getSocket(), welcome4.c_str(), welcome4.length(), MSG_NOSIGNAL) == -1)
			std::perror("Error : Failed to send 004.");
}

void	Command::sendPASSMessage(Client *client)
{
	if (client->getPassCheck() == true)
		return;
	Command::sendNewLine(client);
	std::string pass = "Password required.\nTry /quote PASS <password>\n";
	if (send(client->getSocket(), pass.c_str(), pass.length(), MSG_NOSIGNAL) == -1)
		std::perror("Error : Failed to send PASS message");
	Command::sendNewLine(client);
}

void	Command::checkPassword(Client *client, vector<string> &parsedCommand)
{
	string	passError = ":" + client->getServer()->getNickname() + " 464 * :Password incorrect\n";

	if (client->getPassCheck() == false && parsedCommand[1] == client->getServer()->getPassword())
		client->setPassCheck();
	else
		if (send(client->getSocket(), passError.c_str(), passError.length(), MSG_NOSIGNAL) == -1)
			std::perror("Error : Failed to send password error message");
}

void	Command::handlePing(Client *client, vector<string> &parsedCommand)
{
	std::string pingData;

	pingData += parsedCommand[1] + " ";
	std::string pongResponse = ":" + client->getServer()->getNickname() + " PONG :" + pingData + "\n";
	if (send(client->getSocket(), pongResponse.c_str(), pongResponse.length(), 0) == -1)
		std::perror("Error : Failed to send pong");
}

// ERR_NONICKNAMEGIVEN 			OK
// ERR_ERRONEUSNICKNAME			X
// ERR_NICKNAMEINUSE			X
// ERR_NICKCOLLISION			X
// ERR_UNAVAILRESOURCE			X
// ERR_RESTRICTED				X
void	Command::handleNick(Client *client, vector<string> &parsedCommand)
{
	if (parsedCommand.size() < 2)
	{
		std::string nickError = ":" + client->getServer()->getNickname() + " 431 * :No nickname given\n";
		if (send(client->getSocket(), nickError.c_str(), nickError.length(), MSG_NOSIGNAL) == -1)
			std::perror("Error : Failed to send nickname error message");
	}
	else
	{
		if (client->getNickCheck() == false)
		{
			client->setNickname(parsedCommand[1]);
			client->setNickCheck();
			if (client->getUserCheck())
				Command::sendRPLMessages(client, parsedCommand);
		}
		else //pas bon
		{
			std::string nickError = ":" + client->getServer()->getNickname() + " 433 * " + parsedCommand[1] + " :Nickname is already in use\n";
			if (send(client->getSocket(), nickError.c_str(), nickError.length(), MSG_NOSIGNAL) == -1)
				std::perror("Error : Failed to send nickname error message");
		}
	}
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
			if (client->getNickCheck())
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
