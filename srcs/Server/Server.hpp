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
# include <cstring>
# include <signal.h>

# include "../Client/Client.hpp"

# define DEBUG 0
# define DEBUG2 0

# define MAX_CLIENTS 10
# define MAX_MESSAGE_LENGTH 512


class Client;

class Server
{
private:
	int							_IP;
	int							_epollSocket;
	int							_nbClients;
	int							_serverPort;
	int							_serverSocket;
	std::string					_serverName;
	std::string					_serverPassword;
	struct epoll_event			_serverEvent;	// events management for the server
	struct	sockaddr_in 		_serverAddr;
	std::map<int, Client *>		_clientMap;		// map instead of vector for better efficiency

public:
	Server();
	Server(int port, std::string password);
	~Server();

	int const 						&getIP(void) const;
	std::string const 				&getNickname(void) const;
	std::string const 				&getPassword(void) const;
	int const 						&getPort(void) const;
	int const 						&getSocket(void) const;
	std::map<int, Client *> const	&getClientMap(void) const;


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
						//Gestion du message reçu par un client spécifique
	void				processIncomingData(Client *, const std::string);
	void				handlePing(int, const std::string &);
};

std::ostream & operator<<(std::ostream & lhs, Server const &);

#endif
