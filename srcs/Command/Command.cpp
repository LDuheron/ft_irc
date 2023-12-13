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
	registerCommand("PRIVMSG", handlePrivmsg);

	registerCommand("SHUTDOWN", shutdown);

	registerCommand("QUIT", doNothing);
	registerCommand("WHOIS", doNothing);
	registerCommand("MODE", doNothing);

	registerCommand("JOIN", handleJoin);
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
	// std::cout << "passcheck: " << client->getPassCheck() << "\n";
	// std::cout << "nickcheck: " << client->getNickCheck() << "\n";
	// std::cout << "usercheck: " << client->getUserCheck() << "\n";
	// std::cout << "isconnected: " << client->getIsConnected() << "\n";

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

	std::string capLs = "CAP * LS :none";
	Server::sendMessage(client, capLs);
}

void	Command::sendRPLMessages(Client *client, vector<string> &parsedCommand)
{
	(void)parsedCommand;
	std::string welcome1 = "001 " + client->getNickname() + " :Welcome to the Internet Relay Network " + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname();
	std::string welcome2 = "002 " + client->getNickname() + " :Your host is " + client->getServer()->getNickname()+ ", running version " + SERVER_VERSION;
	std::string welcome3 = "003 " + client->getNickname() + " :This server was created 2023/11/20";
	std::string welcome4 = "004 " + client->getNickname() + " :" + client->getServer()->getNickname() + " 1.0 o itkol";

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
	string nickError = ":" + client->getServer()->getNickname() + " 432 * :Erroneus nickname\n";
	bool res = true;

	if (nickname.empty())
		res = false;
	if (nickname.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`_^{|}-") != std::string::npos)
		res = false;
	if (nickname == "anonymous" || nickname == "server" || nickname == "operator" || nickname == "root" || nickname == "admin" || nickname == "sysop" || nickname == "system")
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
		Server::sendMessageUser(client, nickMessage);
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

	std::cout << "Channel name : " << channelName << "\n";

	std::map<string, Channel *>::iterator it = channelMap.find(channelName);
	if (it != channelMap.end())
		channelMap[channelName]->addMember(client);
	else
	{
		newChannel = new Channel(channelName);

		newChannel->addMember(client);
		newChannel->addOperator(client);
		channelMap[channelName] = newChannel;
	}
	// string message = "JOIN " + channelName;
	// Server::sendMessageUser(client, message);
	sendJoinReply(client, channelMap[channelName]);
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
	(void)sender;
	(void)channel;
	(void)message;
}

static void	msgUser(Client *sender, Client *reciever, string &message)
{
	message = ":" + sender->getNickname() + "!" + sender->getUsername() + "@" + sender->getHostname() + " PRIVMSG " + reciever->getNickname() + " :" + message + "\r\n";
	Server::sendMessageRaw(reciever, message);
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
		std::map<string, Client *>::iterator itClient = server->getClientMapStr().find(parsedCommand[1]);
		if (itClient != server->getClientMapStr().end())
			msgUser(client, itClient->second, message);
		else
			Server::sendMessage(client, error);
	}

	// for (std::map<string, Channel *>::iterator itChan = serv->getChannelMap().begin(); itChan != serv->getChannelMap().end(); itChan++)
	// {
	// 	if ((*itChan)->getName() == parsedCommand[1])
	// 	{
	// 		if ((*itChan)->isMember(client))
	// 			destinataire = parsedCommand[1];
	// 		break;
	// 	}
	// }
	// if (!destinataire.empty())
	// {
	// 	for (std::map<int, Client *>::const_iterator itClient = serv->getClientMap().begin(); itClient != serv->getClientMap().end(); itClient++)
	// 	{
	// 		if (itClient->second->getNickname() == parsedCommand[1])
	// 		{
	// 			destinataire = parsedCommand[1];
	// 			break ;
	// 		}
	// 	}
	// }
	// if (destinataire.empty())
	// {
	// 	std::string msg = "PRIVMSG " + parsedCommand[1] + " :" + parsedCommand[2] + "\r\n";
	// 	send(serv->getSocket(), msg.c_str(), msg.length(), 0);
	// }
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
