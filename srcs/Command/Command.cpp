#include "Command.hpp"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <ios>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

// Constructor -----------------------------------------------------------------

Command::Command()
{
	registerCommand("CAP LS", sendCAPLs);
	registerCommand("CAP END", sendPASSMessage);
	registerCommand("PASS", checkPassword);
	registerCommand("PING", handlePing);
	registerCommand("NICK", handleNick);
	registerCommand("USER", handleUser);

	// //display map for debugging
	// for (std::map<string, CommandFunction>::iterator it = this->_commandMap.begin(); it != this->_commandMap.end(); ++it)
	// 	std::cout << it->first << "\n";
	// registerCommand("JOIN", handleJoin);
}

// Destructor ------------------------------------------------------------------

// Accessors -------------------------------------------------------------------

// Overload --------------------------------------------------------------------

// Functions -------------------------------------------------------------------

void			Command::registerCommand(const std::string &command, CommandFunction function)
{
	this->_commandMap[command] = function;
}

vector<string> splitString(const string &command, const string &delimiter)
{
	vector<string> result;

	size_t	pos = 0;
	size_t	end = command.find(delimiter);

	while (end != std::string::npos)
	{
		result.push_back(command.substr(pos, end - pos));
		pos = end + delimiter.size();
		end = command.find(delimiter, pos);
	}
	if (pos < command.size())
		result.push_back(command.substr(pos, end - pos));
	return (result);
}

vector<vector<string>>	Command::parseLine(const std::string &command)
{
	vector<vector<string>>	parsedLines;
	vector<string>			line = splitString(command, "\r\n");
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
		parsedLines.push_back(parsedCommand);
		line.erase(line.begin());
	}
	std::cout << "\n===== Parsed Line =====\n";
	for(std::vector<std::vector<std::string>>::iterator it = parsedLines.begin(); it != parsedLines.end(); ++it)
	{
		for (std::vector<std::string>::iterator it2 = it->begin(); it2 != it->end(); ++it2)
			std::cout << *it2 << "|";
		std::cout << "\n";
	}
	std::cout << "=======================\n\n";
	return (parsedLines);
}

void	Command::handleCommand(Client *client, const string &message)
{
	vector<vector<string>>	parsedLines = Command::parseLine(message);

	while (!parsedLines.empty())
	{
		vector<string>	&parsedCommand = parsedLines[0];
		std::map<string, CommandFunction>::iterator it = this->_commandMap.find(parsedCommand[0]);
		if (it != this->_commandMap.end())
		{
			if (DEBUG_CMD)
			{
				std::cout << "---------------------\n";
				std::cout << "Command handled:" << parsedCommand[0] << "\n";
				std::cout << "---------------------\n";
			}
			this->_commandMap[parsedCommand[0]](client, parsedCommand);
		}
		else
			std::cerr <<"Error: Unknown command: " << parsedCommand[0] << "\n";
		parsedLines.erase(parsedLines.begin());
	}

}

void	Command::sendCAPLs(Client *client, vector<string> &parsedCommand)
{
	(void)parsedCommand;
	std::string capLs = ":DEFAULT CAP * LS :none\n";
	if (send(client->getSocket(), capLs.c_str(), capLs.length(), MSG_NOSIGNAL) == -1)
		std::perror("Error : Failed to send CAP LS\n");
}

void	Command::sendRPLMessages(Client *client, vector<string> &parsedCommand)
{
	(void)parsedCommand;
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

void	Command::sendPASSMessage(Client *client, vector<string> &parsedCommand)
{
	(void) parsedCommand;	
	std::string pass = "Password required. Try /quote PASS <password>\n";
	if (send(client->getSocket(), pass.c_str(), pass.length(), MSG_NOSIGNAL) == -1)
		std::perror("Error : Failed to send PASS message\n");
}

void	Command::checkPassword(Client *client, vector<string> &parsedCommand)
{
	string	passError = ":localhost 464 * :Password incorrect\n";
	string	passOk = ":localhost 001 * :Welcome to the Internet Relay Network\n";

	if (client->getPassCheck() == false && parsedCommand[1] == client->getServer()->getPassword())
	{
		client->setPassCheck();
		sendRPLMessages(client, parsedCommand);
	}
	else
		if (send(client->getSocket(), passError.c_str(), passError.length(), MSG_NOSIGNAL) == -1)
			std::perror("Error : Failed to send password error message\n");
}

void	Command::handlePing(Client *client, vector<string> &parsedCommand)
{
	std::string pingData;

	while (!parsedCommand.empty())
	{
		pingData += parsedCommand[1] + " ";
	}
	std::string pongResponse = ":localhost PONG :" + pingData + "\n";
	if (send(client->getSocket(), pongResponse.c_str(), pongResponse.length(), 0) == -1)
		std::cerr << "Error : Failed to send pong.\n";
}

void	Command::handleNick(Client *client, vector<string> &parsedCommand)
{
	if (parsedCommand.size() < 2)
	{
		std::string nickError = ":localhost 431 * :No nickname given\n";
		if (send(client->getSocket(), nickError.c_str(), nickError.length(), MSG_NOSIGNAL) == -1)
			std::perror("Error : Failed to send nickname error message\n");
	}
	else
	{
		if (client->getNickCheck() == false)
		{
			client->setNickname(parsedCommand[1]);
			client->setNickCheck();
		}
		else
		{
			std::string nickError = ":localhost 433 * " + parsedCommand[1] + " :Nickname is already in use\n";
			if (send(client->getSocket(), nickError.c_str(), nickError.length(), MSG_NOSIGNAL) == -1)
				std::perror("Error : Failed to send nickname error message\n");
		}
	}
}

void	Command::handleUser(Client *client, vector<string> &parsedCommand)
{
	if (parsedCommand.size() < 5)
	{
		std::string userError = ":localhost 461 * USER :Not enough parameters\n";
		if (send(client->getSocket(), userError.c_str(), userError.length(), MSG_NOSIGNAL) == -1)
			std::perror("Error : Failed to send user error message\n");
	}
	else
	{
		if (client->getPassCheck() == false)
		{
			std::string passError = ":localhost 451 * :You have not registered\n";
			if (send(client->getSocket(), passError.c_str(), passError.length(), MSG_NOSIGNAL) == -1)
				std::perror("Error : Failed to send password error message\n");
		}
		else
		{
			client->setUsername(parsedCommand[1]);
			std::string userOk = ":localhost 001 * :Welcome to the Internet Relay Network\n";
			if (send(client->getSocket(), userOk.c_str(), userOk.length(), MSG_NOSIGNAL) == -1)
				std::perror("Error : Failed to send user ok message\n");
		}
	}
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
