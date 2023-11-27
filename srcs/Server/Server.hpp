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

# define FAIL -1
# define SUCCESS 0
# define ERROR 1

# define MAX_CLIENTS 10
# define MAX_MESSAGE_LENGTH 512


class Client;

class Server
{
	private:
		std::map<int, Client *>		_clientMap;		// map instead of vector for better efficiency
		int							_epollSocket;
		struct epoll_event			_serverEvent; // events managment for the server
		int							_IP;
		std::string					_nickname;
		int							_nbClients;
		std::string					_serverPassword;
		int							_serverPort;
		struct	sockaddr_in 		_serverAddr;
		int							_serverSocket;

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
		/**
		Gestion temporaire du ping

		@param clientSocket : socket du client qui a envoyé le ping
		*/
		void				handlePing(int, const std::string &);
};

std::ostream & operator<<(std::ostream & lhs, Server const &);

#endif
