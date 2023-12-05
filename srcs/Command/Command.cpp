#include "Command.hpp"
#include <cstdio>
#include <cstring>
#include <ios>
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
	// registerCommand("NICK", handleNick);
	// registerCommand("USER", handleUser);

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

vector<vector<string>>	Command::parseLine(const std::string &command)
{
	vector<vector<string>>	parsedLines;
	vector<string>			line;
	std::istringstream		iss(command);
	std::string				token;

	while (iss >> std::skipws >> token)
	{
		while (!token.empty() && (token.back() == '\r' || token.back() == '\n'))
			token.pop_back();
	}

	if (!token.empty())
		line.push_back(token);
	else
	{
		if (!token.empty())
		{
			parsedLines.push_back(line);
			line.clear();
		}
	}
	if (!line.empty())
		parsedLines.push_back(line);
	for(std::vector<std::vector<std::string>>::iterator it = parsedLines.begin(); it != parsedLines.end(); ++it)
	{
		for (std::vector<std::string>::iterator it2 = it->begin(); it2 != it->end(); ++it2)
			std::cout << *it2 << "|";
		std::cout << "\n";
	}
	return (parsedLines);
}
	// while (std::getline(iss, line, '\r' ))
	// {
	// 	if (line.empty())
	// 		continue;
	// 	if (!line.empty() && line.back() == '\n')
	// 		line.pop_back();
	// }

	// vector<string>		singleLine;
	// std::istringstream	lineStream(line);
	// string				token;

	// while (lineStream >> token)
	// 	singleLine.push_back(token);

	// parsedLines.push_back(singleLine);
	// for (std::vector<std::string>::iterator it = singleLine.begin(); it != singleLine.end(); ++it)
	// 	std::cout << *it << "|";
	// return (parsedLines);
// }

	// while (iss >> token)
	// {
	// 	if (!token.empty())
	// 	{
	// 		// if (token.size() > 1 && token[token.size() - 1] == '\r')// && token[token.size() - 1] == '\n')
	// 		if (strchr(token.c_str(), '\r') != NULL)
	// 		{
	// 			std::cout << "BORDELLLLLLLLLLLLLLLLLLLLLLLLLLLLL\n";
	// 			// token.pop_back();
	// 			// token.pop_back();
	// 			singleLine.push_back(token);
	// 			parsedLines.push_back(singleLine);
	// 			std::cout << "Single line: ";
	// 			for (std::vector<std::string>::iterator it = singleLine.begin(); it != singleLine.end(); ++it)
	// 				std::cout << *it << "|";
	// 			singleLine.clear();
	// 		}
	// 		else
	// 			singleLine.push_back(token);
	// 		std::cout << "----- Token: -----" << token << "\n";
	// 		for (std::vector<std::string>::iterator it = singleLine.begin(); it != singleLine.end(); ++it)
	// 			std::cout << *it << "|";
	// 		std::cout << "\n------------------\n";
	// 	}
	// }






	// Merge CAP and LS into single token
	// for(std::vector<std::vector<std::string>>::iterator it = parsedLines.begin(); it != parsedLines.end(); ++it)
	// {
	// 	if (it->size() > 1 && (*it)[0] == "CAP")
	// 	{
	// 		(*it)[1] = "CAP " + (*it)[1];
	// 		it->erase(it->begin());
	// 	}
	// }



	// if (parsedLines[0] == "CAP")
	// {
	// 	parsedLines[1] = "CAP " + parsedLines[1];
	// 	parsedLines.erase(parsedLines.begin());
	// }

// 	std::cout << "===== parseLine : =====\n";
// 	for (std::vector<std::vector<std::string>>::iterator it = parsedLines.begin(); it != parsedLines.end(); ++it)
// 	{
// 		for (std::vector<std::string>::iterator it2 = it->begin(); it2 != it->end(); ++it2)
// 			std::cout << *it2 << "|";
// 		std::cout << "\n";
// 	}
// 	std::cout << "\n============================\n";
// 	return (parsedLines);
// }

// vector<string>	Command::parseCommand(vector<string> &parsedLines)
// {
// 	vector<string>		parsedCommand;
// 	string				token;



// 	// while (iss >> token)
// 	// 	parsedCommand.push_back(token);
// 	// if (!parsedCommand.empty() && parsedCommand[0] == "CAP")
// 	// {
// 	// 	parsedCommand[1] = "CAP " + parsedCommand[1];
// 	// 	parsedCommand.erase(parsedCommand.begin());
// 	// }
// 	std::cout << "----- parseCommand : ------\n";
// 	for (std::vector<std::string>::iterator it = parsedCommand.begin(); it != parsedCommand.end(); ++it)
// 		std::cout << *it << "|";
// 	std::cout << "\n---------------------------\n";
// 	parsedLines.erase(parsedLines.begin());
// 	return (parsedCommand);
// }

void	Command::handleCommand(Client *client, const string &message)
{
	vector<vector<string>>	parsedLines = Command::parseLine(message);
	int i = -1;

	while (!parsedLines.empty())
	{
		vector<string>	parsedCommand = parsedLines[++i];
		const std::string &command = parsedCommand[0];
		std::map<string, CommandFunction>::iterator it = this->_commandMap.find(command);
		if (it != this->_commandMap.end())
		{
			std::cout << "Command handled:" << command << "\n";
			this->_commandMap[command](client, parsedCommand);
		}

		else
			std::cerr <<"Error: Unknown command: " << command << "\n";
	}

}

void	Command::sendCAPLs(Client *client, vector<string> &parsedCommand)
{
	std::string capLs = ":DEFAULT CAP * LS :none\n";
	if (send(client->getSocket(), capLs.c_str(), capLs.length(), MSG_NOSIGNAL) == -1)
		std::perror("Error : Failed to send CAP LS\n");
	parsedCommand.erase(parsedCommand.begin());
	std::cout << "CAP LS sent\n";
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

	std::cout << "RPL messages sent\n";
}

void	Command::sendPASSMessage(Client *client, vector<string> &parsedCommand)
{
	(void) parsedCommand;	
	std::string pass = "Password required. Try /quote PASS <password>\n";
	if (send(client->getSocket(), pass.c_str(), pass.length(), MSG_NOSIGNAL) == -1)
		std::perror("Error : Failed to send PASS message\n");

	std::cout << "PASS message sent\n";
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
	parsedCommand.erase(parsedCommand.begin(), parsedCommand.begin() + 1);
}

void	Command::handlePing(Client *client, vector<string> &parsedCommand)
{
	std::string pingData;

	parsedCommand.erase(parsedCommand.begin());
	while (!parsedCommand.empty())
	{
		pingData += parsedCommand[0] + " ";
		parsedCommand.erase(parsedCommand.begin());
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
	parsedCommand.erase(parsedCommand.begin(), parsedCommand.begin() + 1);
		std::cout << "----- handleNick : ------\n";
	for (std::vector<std::string>::iterator it = parsedCommand.begin(); it != parsedCommand.end(); ++it)
		std::cout << *it << "|";
	std::cout << "\n---------------------------\n";
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
	parsedCommand.erase(parsedCommand.begin(), parsedCommand.begin() + 1);
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
