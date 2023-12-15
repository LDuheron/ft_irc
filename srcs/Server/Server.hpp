#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

# include <arpa/inet.h>
# include <fcntl.h>
# include <iostream>
# include <netinet/in.h>
# include <string>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <unistd.h>
# include <map>
# include <vector>
# include <cstring>
# include <signal.h>
# include <sstream>
# include <functional>

# include "../Client/Client.hpp"
# include "../Message/Message.hpp"
# include "../Channel/Channel.hpp"
# include "../Command/Command.hpp"

# define DEBUG 0
# define DEBUG2 0
# define DEBUG_CMD 0

# define SERVER_VERSION "0.5"

# define HOST_NAME_MAX 255
# define MAX_CLIENTS 10
# define MAX_MESSAGE_LENGTH 512

# define DELIMITER "\n" // delimiter should be \r\n but in the test with nc (cf subject) they use \n only

class Channel;
class Client;
class Command;
class Message;

using std::string;

class Server
{
private:
	int							_IP;
	int							_epollSocket;
	int							_nbClients;
	int							_serverPort;
	int							_serverSocket;
	string						_serverName;
	string						_serverPassword;
	struct epoll_event			_serverEvent;	// events management for the server
	struct	sockaddr_in 		_serverAddr;
	std::map<int, Client *>		_clientMap;
	std::map<string, Client *>	_clientMapStr;
	std::map<string, Channel *>	_channelMap;
	Command						*_command;
	bool						_shutdown;

public:
	Server();
	Server(int port, string password);
	~Server();

	int const 					&getIP(void) const;
	string const 				&getNickname(void) const;
	string const 				&getPassword(void) const;
	int const 					&getPort(void) const;
	int const 					&getSocket(void) const;
	std::map<int, Client *>		&getClientMap(void);
	std::map<string, Client *>	&getClientMapStr(void);
	std::map<string, Channel *>	&getChannelMap(void);
	void						setShutdown(bool);

	void				start();

	void				setSocket(int);

	void				init_serverAddr(void);
	void				init_server(void);
	void				loop(void);

	void				handleNewClient(void);

	void				removeClient(Client *);
	void				handleClientError(Client *);
	void				handleClientDisconnection(Client *);
						//Gestion des messages reçus
	void				handleClientEvent(Client *);

	static void			sendMessage(Client *, string &);
	static void			sendMessageUser(Client *, string &);
	static void			sendMessageRaw(Client *, string &);
	static void			sendMessageChannel(Channel *, string &, Client *client);
};

std::ostream & operator<<(std::ostream & lhs, Server const &);

#endif
