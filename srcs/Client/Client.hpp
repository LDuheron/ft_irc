#pragma once
#include <sys/epoll.h>
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../Server/Server.hpp"

class Client
{
	private:
		int					_clientSocket;
		std::string			_nickname;
		std::string			_username;
		struct epoll_event	_event;
		bool				_nickCheck;
		bool				_passCheck;
		// epollfd ??

	public:
		Client(int id);
		~Client();

		int					getSocket(void) const;
		void				setSocket(int);
		const std::string	&getNickname(void) const;
		void 				setNickname(const std::string &);
		bool				getNickCheck(void) const;
		void				setNickCheck(void);
		bool				getPassCheck(void) const;
		void				setPassCheck(void);
		void				setEvent();
		// struct epoll_event	getEvent(void) const; do we need it though
		struct epoll_event	*getEventAddress(void);

		Client &operator=(Client const & rhs);
};

#endif
