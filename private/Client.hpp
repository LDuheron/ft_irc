#pragma once
#include <sys/epoll.h>
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"

class Client
{
	private:
		int					_fd;
		std::string			_nickname;
		std::string			_username;
		int					_id;
		struct epoll_event	_event;
		// epollfd ??

	public:
		Client(int id);
		~Client();

		int			getFd(void) const;
		void		setFd(int fd);
		int			getId(void) const;
		void		setEvent(int fd);

		Client &operator=(Client const & rhs);
};

#endif
