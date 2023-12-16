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
	registerCommand("WHO", doNothing);
	registerCommand("WHOIS", doNothing);
	registerCommand("MODE", handleModes);

	registerCommand("JOIN", handleJoin);
	registerCommand("KICK", handleKick);
	registerCommand("INVITE", handleInvite);

	registerCommand("TOPIC", handleTopic);

	registerCommand("OPER", doNothing);
	registerCommand("QUIT", handleQuit);

	registerCommand("PART", handlePart);
	registerCommand("PRIVMSG", handlePrivmsg);
}

Command::~Command()
{
	this->_commandMap.clear();
}

// Destructor ------------------------------------------------------------------

// Accessors -------------------------------------------------------------------

// Overload --------------------------------------------------------------------

// Functions -------------------------------------------------------------------

void		Command::registerCommand(const std::string &command, CommandFunction function)
{
	this->_commandMap[command] = function;
}

static vector<string>	splitString(Client *client, const string &command, const string &delimiter)
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

void		Command::execCommandBuffered(Client *client)
{
	while (!this->_commandBuffer.empty())
	{
		this->_commandMap[this->_commandBuffer[0][0]](client, this->_commandBuffer[0]);
		this->_commandBuffer.erase(this->_commandBuffer.begin());
	}
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
	if (!client->getIsConnected() && parsedCommand[0] != "PASS" && parsedCommand[0] != "CAP LS" && parsedCommand[0] != "CAP END")
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
			if (!client->getPassCheck())
			{
				this->_commandBuffer.push_back(parsedCommand);
				parsedCommand.clear();
			}
			else
				this->_commandMap[parsedCommand[0]](client, parsedCommand);
		}
	}
	else if (!parsedCommand.empty())
	{
		this->_commandMap[parsedCommand[0]](client, parsedCommand);
		if (client->getPassCheck())
			execCommandBuffered(client);
	}
}

void		Command::execCmds(Client *client, const std::string &command)
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
		if (LOG_OUTPUT)
		{
			std::cout << "\n===== Data received from client : =====\n";
			for (std::vector<std::string>::iterator it2 = parsedCommand.begin(); it2 != parsedCommand.end(); ++it2)
				std::cout << *it2 << "|";
			std::cout << "\n=======================================\n\n";
		}
		exec(client, parsedCommand);
		line.erase(line.begin());
	}
}

void	Command::sendCAPLs(Client *client, vector<string> &parsedCommand)
{
	(void)parsedCommand;

	std::string capLs = "CAP * LS :none";
	Server::sendMessage(client, capLs);
}

void	Command::sendRPLMessages(Client *client, vector<string> &parsedCommand)
{
	(void)parsedCommand;
	std::string welcome1 = "001 " + client->getNickname() + " :Welcome to the Internet Relay Network " + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname();
	std::string welcome2 = "002 " + client->getNickname() + " :Your host is " + client->getServer()->getNickname()+ ", running version " + SERVER_VERSION;
	std::string welcome3 = "003 " + client->getNickname() + " :This server was created 2023/11/20";
	std::string welcome4 = "004 " + client->getNickname() + " :" + client->getServer()->getNickname() + " 1.0 -none- itkol";

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
	std::string pass = "Password required.\nTry /quote PASS <password>";
	Server::sendMessage(client, pass);
	Command::sendNewLine(client);
}

void	Command::checkPassword(Client *client, vector<string> &parsedCommand)
{
	string	passError = "464 * :Password incorrect";

	if (parsedCommand.size() != 2)
	{
		Server::sendMessage(client, passError);
		return;
	}
	if (client->getPassCheck() == false && parsedCommand[1] == client->getServer()->getPassword())
		client->setPassCheck();
	else
		Server::sendMessage(client, passError);
}

void	Command::handlePing(Client *client, vector<string> &parsedCommand)
{
	string	errorMessage = "409 " + client->getUsername() + " :No origin specified";
	if (parsedCommand.size() != 2)
	{
		Server::sendMessage(client, errorMessage);
		return;
	}
	std::string pongResponse = "PONG " + client->getServer()->getNickname() + " :" + parsedCommand[1];
	Server::sendMessage(client, pongResponse);
}

static bool	checkValidNick(Client *client, const string &nickname)	
{
	string nickError = "432 * :Erroneus nickname";
	bool res = true;

	if (nickname.empty())
		res = false;
	if (nickname.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`_^{|}-") != std::string::npos)
		res = false;
	if (nickname == "anonymous" || nickname == "server" || nickname == "operator" || nickname == "root" || nickname == "admin" || nickname == "sysop" || nickname == "system" || nickname == "bot")
		res = false;
	else if (nickname.length() > 9)
		res = false;
	if (!res)
		Server::sendMessage(client, nickError);
	return (res);
}

static bool	checkNickInUse(Client *client, const string &nickname)
{
	string nickError = "433 * " + nickname + " :Nickname is already in use";

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

static void	handleValidNick(Client *client, vector<string> &parsedCommand)
{
	string nickMessage = "NICK " + parsedCommand[1];
	if (client->getNickCheck() && client->getUserCheck())
		Server::sendMessageUser(client, nickMessage); // need to send to all users of the specific channels this user is in + all current privmsg
	client->getServer()->getClientMapStr().erase(client->getNickname());
	client->setNickname(parsedCommand[1]);
	client->getServer()->getClientMapStr()[parsedCommand[1]] = client;
	client->setNickCheck();
	if (client->getUserCheck() && !client->getIsConnected())
		Command::sendRPLMessages(client, parsedCommand);
}

// Need to send nick update to all clients of a channel
// ERR_NICKCOLLISION			FLEMME
// ERR_UNAVAILRESOURCE			FLEMME
// ERR_RESTRICTED				X
void	Command::handleNick(Client *client, vector<string> &parsedCommand)
{
	if (parsedCommand.size() < 2)
	{
		string nickError = "431 * :No nickname given";
		Server::sendMessage(client, nickError);
		return;
	}
	if (parsedCommand.size() > 2)
	{
		string nickError = "432 * :Erroneus nickname";
		Server::sendMessage(client, nickError);
		return;
	}
	if (!checkValidNick(client, parsedCommand[1]))
		return;
	if (checkNickInUse(client, parsedCommand[1]))
		return;
	handleValidNick(client, parsedCommand);
}

// ERR OK
void	Command::handleUser(Client *client, vector<string> &parsedCommand)
{
	if (parsedCommand.size() < 5)
	{
		std::string userError = "461 * USER :Not enough parameters";
		Server::sendMessage(client, userError);
		return;
	}
	else if (client->getUserCheck())
	{
		std::string AlreadyRegistered = "462 * :Unauthorized command (already registered)";
		Server::sendMessage(client, AlreadyRegistered);
		return;
	}
	else
	{
			client->setUsername(parsedCommand[1]);
			if (parsedCommand[4][0] == ':')
			{
				string realname = parsedCommand[4].substr(1, parsedCommand[4].size());
				for (size_t i = 5; i < parsedCommand.size(); ++i)
					realname += " " + parsedCommand[i];
				client->setRealname(realname);
			}
			else
				client->setRealname(parsedCommand[4]);
			client->setUserCheck();
			if (client->getNickCheck() && !client->getIsConnected())
				Command::sendRPLMessages(client, parsedCommand);
	}
}

string 	listCommandMembers(Channel *channel)
{
	string result = "";

	for (vector<Client *>::const_iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); ++it)
	{
		if ((*it)->isOperator(channel))
			result += "@" + (*it)->getNickname() + " ";
		else
			result += (*it)->getNickname() + " ";
	}
	return result;
}

void	sendJoinToAllUsers(Channel *channel, Client *client)
{
	string message = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " JOIN " + channel->getName() + "\r\n";

	for (vector<Client *>::const_iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); ++it)
	{
		if (*it != client)
			Server::sendMessageRaw(*it, message);
	}
}

void	sendJoinReply(Client *client, Channel *channel)
{
	string joinReply = "JOIN " + channel->getName();
	string namesMessage = "353 " + client->getNickname() + " = " + channel->getName() + " :" + listCommandMembers(channel);
	string endOfNames = "366 " + client->getNickname() + " " + channel->getName() + " :End of NAMES list";

	Server::sendMessageUser(client, joinReply);
	Server::sendMessage(client, namesMessage);
	Server::sendMessage(client, endOfNames);
	sendJoinToAllUsers(channel, client);
}

void	Command::handleJoin(Client *client, vector<string> &parsedCommand)
{
	std::map<string, Channel *>	&channelMap = client->getServer()->getChannelMap();
	string						channelName = parsedCommand[1]; //faire un parsing du name ptet
	Channel						*newChannel = NULL;

	if (channelName[0] != '#')
	{
		string error = "403 " + client->getNickname() + " " + channelName + " :invalid Channel Name";
		Server::sendMessage(client, error);
		return;
	}
	std::map<string, Channel *>::iterator it = channelMap.find(channelName);
	if (it != channelMap.end())
	{
		if (channelMap[channelName]->getInviteOnly() == true)
		{
			if (channelMap[channelName]->isInvited(client) == false)
			{
				string error = "473 " + client->getNickname() + " " + channelName + " :Cannot join channel (+i)";
				Server::sendMessage(client, error);
				return;
			}
		}
		if (channelMap[channelName]->getHasPassword() == true)
		{
			if (parsedCommand.size() < 3)
			{
				string error = "461 " + client->getNickname() + " " + channelName + " :Not enough parameters";
				Server::sendMessage(client, error);
				return;
			}
			if (parsedCommand[2] != channelMap[channelName]->getPassword())
			{
				string error = "475 " + client->getNickname() + " " + channelName + " :Cannot join channel (+k)";
				Server::sendMessage(client, error);
				return;
			}
		}
		if (channelMap[channelName]->getHasUserLimit() == true)
		{
			if (channelMap[channelName]->getMembers().size() >= channelMap[channelName]->getUserLimit())
			{
				string error = "471 " + client->getNickname() + " " + channelName + " :Cannot join channel (+l)";
				Server::sendMessage(client, error);
				return;
			}
		}
		channelMap[channelName]->addMember(client);
	}
	else
	{
		newChannel = new Channel(channelName, client->getServer());

		newChannel->addMember(client);
		newChannel->addOperator(client);
		channelMap[channelName] = newChannel;
	}
	sendJoinReply(client, channelMap[channelName]);
}

static void sendPartReply(Client *client, Channel *channel, string &message)
{
	string partReply = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " PART " + channel->getName() + " :" + message + "\r\n";
	for (vector<Client *>::const_iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); ++it)
			Server::sendMessageRaw(*it, partReply);
}

void	Command::handlePart(Client *client, vector<string> &parsedCommand)
{
	std::map<string, Channel *>	&channelMap = client->getServer()->getChannelMap();
	string						channelName = parsedCommand[1];
	string						trailingMessage = "";

	if (channelName[0] != '#')
	{
		string error = "403 " + client->getNickname() + " " + channelName + " :invalid Channel Name";
		Server::sendMessage(client, error);
		return;
	}
	std::map<string, Channel *>::iterator it = channelMap.find(channelName);
	if (it == channelMap.end())
	{
		string error = "403 " + client->getNickname() + " " + channelName + " :No such channel";
		Server::sendMessage(client, error);
		return;
	}
	Channel *channel = channelMap[channelName];
	if (client->isOperator(channel))
		channel->removeOperator(client);

	if (parsedCommand.size() > 2 && parsedCommand[2][0] == ':')
	{
		trailingMessage = parsedCommand[2].substr(1, parsedCommand[2].size());
		if (parsedCommand.size() > 3)
		{
			for	(size_t i = 3; i < parsedCommand.size(); ++i)
				trailingMessage += " " + parsedCommand[i];
		}
	}
	sendPartReply(client, channel, trailingMessage);
	channel->removeMember(client);
	if (channel->getMembers().empty())
	{
		channelMap.erase(channelName);
		delete channel;
	}
}

static string	parseMsg(vector<string> &parsedCommand)
{
	string message = parsedCommand[2];

	if (message[0] == ':')
	{
		message = message.substr(1, message.size());
		for (size_t i = 3; i < parsedCommand.size(); ++i)
			message += " " + parsedCommand[i];
	}
	return message;
}

static void	msgChannel(Client *sender, Channel *channel, string &message)
{
	for (std::vector<Client*>::const_iterator itClientChan = channel->getMembers().begin(); itClientChan != channel->getMembers().end(); itClientChan++)
	{
		if ((*itClientChan)->getNickname() == sender->getNickname())
		{
			message = ":" + sender->getNickname() + "!" + sender->getUsername() + "@" + sender->getHostname() + " PRIVMSG " + channel->getName() + " :" + message + "\r\n";
			for (vector<Client *>::const_iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); ++it)
			{
				if (*it != sender)
					Server::sendMessageRaw(*it, message);
			}
			break ;
		}
	}
}

static void	msgUser(Client *sender, Client *receiver, string &message)
{
	message = ":" + sender->getNickname() + "!" + sender->getUsername() + "@" + sender->getHostname() + " PRIVMSG " + receiver->getNickname() + " :" + message + "\r\n";
	Server::sendMessageRaw(receiver, message);
}

void	Command::handlePrivmsg(Client *client, vector<string> &parsedCommand)
{
	string	error = "401 " + client->getNickname() + " " + parsedCommand[1] + " :No such nick/channel";

	Server	*server = client->getServer();
	string	message = parseMsg(parsedCommand);

	if (parsedCommand[1][0] == '#')
	{
		std::map<string, Channel *>::iterator itChan = server->getChannelMap().find(parsedCommand[1]);
		if (itChan != server->getChannelMap().end())
			msgChannel(client, itChan->second, message);
		else
			Server::sendMessage(client, error);
	}
	else
	{
		if (parsedCommand[1] == "bot")
		{
			client->getServer()->getBot()->sendFacts(client);
		}
		else
		{
			std::map<string, Client *>::iterator itClient = server->getClientMapStr().find(parsedCommand[1]);
			if (itClient != server->getClientMapStr().end())
				msgUser(client, itClient->second, message);
			else
				Server::sendMessage(client, error);
		}
	}
}

void setUserMode(Client *client, vector<string> &parsedCommand)
{
	while (!parsedCommand[2].empty())
	{
		if (parsedCommand[2][0] == '+')
		{
			for (size_t i = 1; i < parsedCommand[2].size(); ++i)
			{
				if (parsedCommand[2][i] == '+' || parsedCommand[2][i] == '-')
					break;
				client->addMode(parsedCommand[2][i]);
			}
		}
		else if (parsedCommand[2][0] == '-')
		{
			for (size_t i = 1; i < parsedCommand[2].size(); ++i)
			{
				if (parsedCommand[2][i] == '+' || parsedCommand[2][i] == '-')
					break;
				client->removeMode(parsedCommand[2][i]);
			}
		}
		parsedCommand[2] = parsedCommand[2].substr(1);
	}
}

void	executeMode(Channel *channel, vector<string> &parsedCommand, char command)
{
	switch (command) {
		case 'k':
			if (parsedCommand.size() < 4)
			{
				channel->removeMode('k');
				return;
			}
			channel->setHasPassword(true);
			channel->setPassword(parsedCommand[3]);
			parsedCommand.erase(parsedCommand.begin() + 3);
			break;
		case 'i':
			channel->setInviteOnly(true);
			break;
		case 'l':
			if (parsedCommand.size() < 4 || parsedCommand[3].find_first_not_of("0123456789") != std::string::npos)
			{
				channel->removeMode('l');
				return;
			}
			channel->setHasUserLimit(true);
			channel->setUserLimit(std::stoi(parsedCommand[3]));
			parsedCommand.erase(parsedCommand.begin() + 3);
			break;
		case 't':
			channel->setTopicProtection(true);
			break;
		case 'o':
			if (parsedCommand.size() < 4)
				return;
			std::map<string, Client *>::iterator it = channel->getServer()->getClientMapStr().find(parsedCommand[3]);
			if (it == channel->getServer()->getClientMapStr().end())
				return;
			channel->addOperator(channel->getServer()->getClientMapStr()[parsedCommand[3]]);
			parsedCommand.erase(parsedCommand.begin() + 3);
			break;
	}
}

void	removeMode(Channel *channel, vector<string> &parsedCommand, char command)
{
	switch (command) {
		case 'k':
			channel->setHasPassword(false);
			break;
		case 'i':
			channel->setInviteOnly(false);
			break;
		case 'l':
			channel->setHasUserLimit(false);
			break;
		case 't':
			channel->setTopicProtection(false);
			break;
		case 'o':
			if (parsedCommand.size() < 4)
				return;
			std::map<string, Client *>::iterator it = channel->getServer()->getClientMapStr().find(parsedCommand[3]);
			if (it == channel->getServer()->getClientMapStr().end())
				return;
			channel->removeOperator(channel->getServer()->getClientMapStr()[parsedCommand[3]]);
			parsedCommand.erase(parsedCommand.begin() + 3);
			break;
	}
}

void	setChannelMode(Channel *channel, vector<string> &parsedCommand)
{
	while (!parsedCommand[2].empty())
	{
		if (parsedCommand[2][0] == '+')
		{
			for (size_t i = 1; i < parsedCommand[2].size(); ++i)
			{
				if (parsedCommand[2][i] == '+' || parsedCommand[2][i] == '-')
					break;
				channel->addMode(parsedCommand[2][i]);
				executeMode(channel, parsedCommand, parsedCommand[2][i]);
			}
		}
		else if (parsedCommand[2][0] == '-')
		{
			for (size_t i = 1; i < parsedCommand[2].size(); ++i)
			{
				if (parsedCommand[2][i] == '+' || parsedCommand[2][i] == '-')
					break;
				channel->removeMode(parsedCommand[2][i]);
				removeMode(channel, parsedCommand, parsedCommand[2][i]);
			}
		}
		parsedCommand[2] = parsedCommand[2].substr(1);
	}
}

void	Command::handleModes(Client *client, vector<string> &parsedCommand)
{
	if (parsedCommand.size() < 2)
	{
		string error = "461 * MODE :Not enough parameters";
		Server::sendMessage(client, error);
		return;
	}
	string name = parsedCommand[1];
	if (name[0] != '#')
	{
		if (client->getNickname() != name)
		{
			string error = "502 * :Cant change mode for other users";
			Server::sendMessage(client, error);
			return;
		}
		if (parsedCommand.size() == 2)
		{
			string modes = "221 " + client->getNickname() + " +" + client->getModesString();
			return;
		}
		else if (parsedCommand[2].find_first_not_of("+-i") != std::string::npos)
		{
			string error = "472 " + client->getNickname() + " " + parsedCommand[2] + " :is unknown mode char to me";
			Server::sendMessage(client, error);
			return;
		}
		setUserMode(client, parsedCommand);
		string modes = "221 " + client->getNickname() + " +" + client->getModesString();
		Server::sendMessage(client, modes);
		return;
	}
	std::map<string, Channel *>	&channelMap = client->getServer()->getChannelMap();
	std::map<string, Channel *>::iterator it = channelMap.find(name);
	if (it == channelMap.end())
	{
		string error = "403 " + client->getNickname() + " " + name + " :No such channel";
		Server::sendMessage(client, error);
		return;
	}
	if (!client->isOperator(it->second) && parsedCommand.size() > 2)
	{
		string error = "482 " + client->getNickname() + " :You're not channel operator";
		Server::sendMessage(client, error);
		return;
	}
	if (parsedCommand.size() == 2)
	{
		string modes = "324 " + client->getNickname() + " " + name + " +" + it->second->getModesString();
		Server::sendMessage(client, modes);
		return;
	}
	else if (parsedCommand[2].find_first_not_of("+-itkol") != std::string::npos)
	{
		string error = "472 " + client->getNickname() + " " + parsedCommand[2] + " :is unknown mode char to me";
		Server::sendMessage(client, error);
		return;
	}
	Channel *channel = channelMap[name];
	setChannelMode(channel, parsedCommand);
	string modes = "324 " + client->getNickname() + " " + channel->getName() + " +" + channel->getModesString();
	Server::sendMessage(client, modes);
	string msg = "324 " + client->getNickname() + " " + channel->getName() + " +" + channel->getModesString();
	Server::sendMessageChannel(channel, msg, client);
	return;
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

static void sendKickReply(Client *client, Channel *channel, string &message)
{
	string kickReply = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " " + message + "\r\n";
	for (vector<Client *>::const_iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); ++it)
			Server::sendMessageRaw(*it, kickReply);
}

// KICK <channel> <user> :<reason>
void	Command::handleKick(Client *client, vector<string> &parsedCommand)
{
	string	trailingMessage = "";

	if (parsedCommand.size() < 3)
	{
		string error = "461 " + client->getNickname() + " KICK :Not enough parameters";
		Server::sendMessage(client, error);
		return;
	}

	std::map<string, Channel *>::iterator itChannel = client->getServer()->getChannelMap().find(parsedCommand[1]);
	if (itChannel == client->getServer()->getChannelMap().end())
	{
		string error = "403 " + client->getNickname() + " " + parsedCommand[1] + " :No such channel";
		Server::sendMessage(client, error);
		return;
	}
	if (!client->isOperator(itChannel->second))
	{
		string error = "482 " + client->getNickname() + " :You're not channel operator";
		Server::sendMessage(client, error);
		return;
	}
	std::map<string, Client *>::iterator itClient = client->getServer()->getClientMapStr().find(parsedCommand[2]);
	if (itClient == client->getServer()->getClientMapStr().end())
	{
		string error = " " + client->getNickname() + " " + parsedCommand[2] + " :No such nick/channel";
		Server::sendMessage(client, error);
		return;
	}
	if (!(*itChannel).second->isMember(itClient->second))
	{
		string error = "441 " + client->getNickname() + " " + parsedCommand[2] + " :is not on channel " + parsedCommand[1];
		Server::sendMessage(client, error);
		return;
	}
	std::string msg = "KICK " + itChannel->second->getName() + " " + parsedCommand[2] + " ";
		if (parsedCommand.size() > 2 && parsedCommand[2][0] == ':')
	{
		trailingMessage = parsedCommand[2].substr(1, parsedCommand[2].size());
		if (parsedCommand.size() > 3)
		{
			for	(size_t i = 3; i < parsedCommand.size(); ++i)
				trailingMessage += " " + parsedCommand[i];
		}
	}
	msg += ":" + trailingMessage;
	sendKickReply(client, itChannel->second, msg);
	itChannel->second->removeMember(client);
	if (itChannel->second->getMembers().empty())
	{
		client->getServer()->getChannelMap().erase(itChannel->first);
		delete itChannel->second;
	}
}

void	Command::handleInvite(Client *sender, vector<string> &parsedCommand)
{
	std::map<string, Channel *>::iterator itChannel = sender->getServer()->getChannelMap().find(parsedCommand[2]);
	if (itChannel == sender->getServer()->getChannelMap().end())
	{
		std::string msg = "403 " + sender->getNickname() + " " + parsedCommand[2] + " :No such channel";
		Server::sendMessage(sender, msg);
		return;
	}
	if (!sender->isOperator(itChannel->second))
	{
		std::string msg = "482 " + sender->getNickname() + " :You're not channel operator";
		Server::sendMessage(sender, msg);
		return;
	}
	std::map<string, Client *>::iterator itClient = sender->getServer()->getClientMapStr().find(parsedCommand[1]);
	if (itClient == sender->getServer()->getClientMapStr().end())
	{
		std::string msg = "401 " + sender->getNickname() + " " + parsedCommand[1] + " :No such nick/channel";
		return;
	}
	Channel	*channel = itChannel->second;
	Client	*receiver = itClient->second;

	if (channel->isInvited(receiver) == false)
		channel->inviteMember(receiver);
	std::string msgSender = "341 " + sender->getNickname() + " " + parsedCommand[1] + " " + parsedCommand[2];
	std::string msgRecv = ":" + sender->getNickname() + "!" + sender->getUsername() + "@" + sender->getHostname() + " INVITE " + parsedCommand[1] + " " + parsedCommand[2] + "\r\n";
	Server::sendMessage(sender, msgSender);
	Server::sendMessageRaw(receiver, msgRecv);
}

void	Command::handleQuit(Client *client, vector<string> &parsedCommand)
{
	(void)parsedCommand;
	std::map<string, Channel *>	&channelMap = client->getServer()->getChannelMap();
	for (std::map<string, Channel *>::iterator it = channelMap.begin(); it != channelMap.end(); ++it)
	{
		if (it->second->isMember(client))
		{
			if (it->second->isOperator(client))
				it->second->removeOperator(client);
			it->second->removeMember(client);
			if (it->second->getMembers().empty())
			{
				channelMap.erase(it->first);
				delete it->second;
			}
		}
	}
}

// /topic <channel> <new Topic>
void	Command::handleTopic(Client *client, vector<string> &parsedCommand)
{
	if (parsedCommand.size() < 2)
	{
		string error = "461 " + client->getNickname() + " TOPIC :Not enough parameters";
		Server::sendMessage(client, error);
		return;
	}

	std::map<string, Channel *>::iterator itChannel = client->getServer()->getChannelMap().find(parsedCommand[1]);
	if (itChannel != client->getServer()->getChannelMap().end())
	{
		if (parsedCommand.size() == 2) // afficher topic
		{
			if (!(itChannel->second->getTopic().empty()))
			{
				std::string msg = "332 " + client->getNickname() + " " + itChannel->second->getName() + " :" + itChannel->second->getTopic();
				Server::sendMessage(client, msg);
				return;
			}
			std::string msg = "331 " + client->getNickname() + " " + itChannel->second->getName() + " :No topic is set";
			Server::sendMessage(client, msg);
			return;
		}
		if (parsedCommand.size() == 3)	// set new topic 
		{
			if (!client->isOperator(itChannel->second))
			{
				std::string msg = "482 " + client->getNickname() + " :You're not channel operator";
				Server::sendMessage(client, msg);
				return;
			}
			itChannel->second->setTopic(parsedCommand[2]);
			std::string msg = " TOPIC " + itChannel->second->getName() + " :" + itChannel->second->getTopic();
			Server::sendMessageChannel(itChannel->second, msg, NULL);
		}
	}
}
