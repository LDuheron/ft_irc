#pragma once
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../Server/Server.hpp"

class Channel;
class Server;
class Command; //////// DEBUG

class Client
{
	private:
		int						_clientSocket;
		std::string				_nickname;
		std::string				_username;
		std::string				_hostname;
		std::string				_realname;
		bool					_nickCheck;
		bool					_passCheck;
		struct epoll_event		_event;
		Server					*_server;

		bool				_incomplete; //add to message class
		bool				_capLSsent;


	public:
		Client(int, Server *);
		~Client();

		void				setIncomplete(bool);
		bool				getIncomplete(void) const;
		void				setCapLSsent(bool);
		bool				getCapLSsent(void) const;


		int					getSocket(void) const;
		void				setSocket(int);
		const std::string	&getNickname(void) const;
		void 				setNickname(const std::string &);
		const std::string	&getUsername(void) const;
		void 				setUsername(const std::string &);
		const std::string	&getHostname(void) const;
		void 				setHostname(const std::string &);
		const std::string	&getRealname(void) const;
		void 				setRealname(const std::string &);
		bool				getNickCheck(void) const;
		void				setNickCheck(void);
		bool				getPassCheck(void) const;
		void				setPassCheck(void);
		void				setEvent();
		struct epoll_event	*getEventAddress(void);
		Server				*getServer(void) const;

		Client &operator=(Client const & rhs);
};

#endif
