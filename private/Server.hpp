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

# include <iostream>
# include <limits.h>
# include <stdlib.h>
# include <string>
# include <vector>

# define DEBUG 0

# define SUCCESS 0
# define ERROR 1

# define MAX_CLIENTS 10

class Server
{
	private:
		int					_IP;
		std::string			_password;
		int					_port;
		std::vector<int>	_socket; // [0] = fd du server, else fd clients
		// structure avec les fds de select / pol / epoll ?

	public:
		Server();
		Server(int port, std::string password);
		Server(Server const & src);
		~Server();

		Server &	operator=(Server const & rhs);
	
		std::string const 	&getPassword(void) const;
		int const 			&getIP(void) const;
		int const 			&getPort(void) const;

		void				init_server(void);
		void				loop(void);

};

std::ostream & operator<<(std::ostream & lhs, Server const & rhs);

#endif
