/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lduheron <lduheron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 22:12:15 by lduheron          #+#    #+#             */
/*   Updated: 2023/10/27 15:37:58 by lduheron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <fstream>
# include <iostream>
# include <limits.h>
# include <netinet/in.h>
# include <stdlib.h>
# include <string>
# include <sys/socket.h>
// # include <sys/types.h>
# include <vector>

# define DEBUG 0

# define FAIL -1
# define SUCCESS 0
# define ERROR 1

# define MAX_CLIENTS 10

class Server
{
	private:
		int					_IP;
		std::string			_nickname; // length max 9
		std::string			_password;
		int					_port;
		struct	sockaddr_in _serverAddr;
		int					_socket;
		/////// [0] = fd du server, else fd clients
		// structure avec les fds de select / pol / epoll ?

	public:
		Server();
		Server(int port, std::string password);
		Server(Server const & src);
		~Server();

		Server &	operator=(Server const & rhs);

		int const 			&getIP(void) const;
		std::string const 	&getNickname(void) const;
		std::string const 	&getPassword(void) const;
		int const 			&getPort(void) const;
		int const 			&getSocket(void) const;

		void				setSocket(int newSocket);

		void				init_serverAddr(void);
		void				init_server(void);
		void				loop(void);

};

std::ostream & operator<<(std::ostream & lhs, Server const & rhs);

#endif
