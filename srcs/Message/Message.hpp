#pragma once
#ifndef MESSAGE_H
# define MESSAGE_H

// # include <map>

# include "../Server/Server.hpp"

class Channel;
class Command;
class Server;
class Client;

// https://www.alien.net.au/irc/irc2numerics.html

// :server 001 <nick> :Welcome to the <network> Network, <nick>[!<user>@<host>]

enum
{
	RPL_WELCOME = 001,
	RPL_YOURHOST = 002,
	RPL_CREATED = 003,
	TPL_MYINFO = 004,
// message different nickname. 
};

class Message
{
public:

	Message();
	Message(const Message &toCopy);
	~Message();

	Message	&operator=(const Message &rhs);

protected:

	Client *_sender;
	Client *_receiver;
	// std::string _payload ?
	// commande => pointeur sur fonction
	// std::map<int, Client *> _clientMap;

};
#endif
