#pragma once
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../Server/Server.hpp"

class Client
{
	private:
		int					_clientSocket;
		std::string			_nickname;
		std::string			_username;
		bool				_nickCheck;
		bool				_passCheck;
		struct epoll_event	_event;

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
		struct epoll_event	*getEventAddress(void);

		Client &operator=(Client const & rhs);
};

#endif
