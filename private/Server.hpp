
#ifndef SERVER_HPP
# define SERVER_HPP

# include <arpa/inet.h>
# include <csignal>
# include <sys/epoll.h>
# include <fcntl.h>
# include <fstream>
# include <iostream>
# include <limits.h>
# include <netinet/in.h>
# include <stdlib.h>
# include <stdio.h>
# include <string>
# include <string.h>
# include <sys/socket.h>
# include <unistd.h> 
# include <vector>

# include "Client.hpp"

# define DEBUG 1

# define FAIL -1
# define SUCCESS 0
# define ERROR 1

# define MAX_CLIENTS 10

class Client;

class Server
{
	private:
		// std::vector<int>		_allSockets;
		int						_epollFd;
		struct	epoll_event		_epollEvent;
		int						_IP; //???
		std::string				_nickname; // length max 9
		int						_nbClients;
		std::string				_password;
		int						_port;
		struct	sockaddr_in 	_serverAddr;
		int						_socket;
		std::vector<Client*>	_allClients;

		/////// [0] = fd du server, else fd clients
		// structure avec les fds de select / pol / epoll ?

	public:
		Server();
		Server(int port, std::string password);
		Server(Server const & src);
		~Server();

		Server &			operator=(Server const & rhs);

		int const 			&getIP(void) const;
		std::string const 	&getNickname(void) const;
		std::string const 	&getPassword(void) const;
		int const 			&getPort(void) const;
		int const 			&getSocket(void) const;

		void				setSocket(int newSocket);

		void				init_serverAddr(void);
		void				init_server(void);
		void				loop(void);

		void				check_inactivity(void);
		void				handleNewClient(void);
		void				handleNewRequest(void);
};

std::ostream & operator<<(std::ostream & lhs, Server const & rhs);

#endif
