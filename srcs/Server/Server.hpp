#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

# include <arpa/inet.h>
# include <csignal>
# include <fcntl.h>
# include <fstream>
# include <iostream>
# include <limits.h>
# include <netinet/in.h>
# include <stdlib.h>
# include <stdio.h>
# include <string>
# include <string.h>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <unistd.h>
# include <vector>
# include <system_error>
# include <map>

// # include "../Channel/Channel.hpp"
// # include "Message.hpp"
# include "../Client/Client.hpp"

# define DEBUG 0
# define DEBUG2 1

# define FAIL -1
# define SUCCESS 0
# define ERROR 1

# define MAX_CLIENTS 10
# define MAX_MESSAGE_LENGTH 512


class Client;

class Server
{
	private: // allFd unnecessary ?
		std::map<int, Client *>		_clientMap;		// map instead of vector for better efficiency
		// std::vector<int>			_allFd;			// _allFd[0] = fd du server, else fd clients
	int								_epollFd;
		struct epoll_event			_serverEvent; // events managment for the server
		int							_IP;			// ???
		std::string					_nickname;		// length max 9
		int							_nbClients;
		std::string					_serverPassword;
		int							_serverPort;
		struct	sockaddr_in 		_serverAddr;
		int							_serverSocket;

	public:
		Server();
		Server(int port, std::string password);
		// Server(Server const & src);
		~Server();

		// Server &			operator=(Server const & rhs);

		int const 					&getIP(void) const;
		std::string const 			&getNickname(void) const;
		std::string const 			&getPassword(void) const;
		int const 					&getPort(void) const;
		int const 					&getSocket(void) const;
		std::map<int, Client *> const	&getClientMap(void) const;


		void				setSocket(int newSocket);

		void				init_serverAddr(void);
		void				init_server(void);
		void				loop(void);

		void				check_inactivity(void);
		void				handleNewClient(void);
		void				handleNewRequest(void);

		//Gestion temporaire de tous les messages reçus
		void				handleClientEvent(Client *client);
		//Gestion temporaire du message reçu par un client spécifique
		void				processIncomingData(Client *client, const std::string message);
		/**
		Gestion temporaire du ping

		@param clientSocket : socket du client qui a envoyé le ping
		*/
		void				handlePing(int clientSocket, const std::string &pingData);
};

std::ostream & operator<<(std::ostream & lhs, Server const & rhs);

#endif
