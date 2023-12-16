#pragma once
#include <set>
#include <string>
#include <unistd.h>
#include <vector>
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../Server/Server.hpp"

class Server;
class Channel;

class Client
{
	private:
		int						_clientSocket;
		std::string				_nickname;
		std::string				_username;
		std::string				_hostname;
		std::string				_realname;
		bool					_nickCheck;
		bool					_userCheck;
		bool					_passCheck;
		struct epoll_event		_event;
		Server					*_server;
		bool					_incomplete; //add to message class
		std::string				_incompleteMessage; //add to message class
		bool					_capLSsent;
		bool					_askPassword;
		bool					_isConnected;
		std::set<char>			_modes;
	
		// bool					_isServOperator; /////// /oper / die /quit /rehash

	public:
		Client(int, Server *);
		~Client();

		void				setIncomplete(bool);
		bool				getIncomplete(void) const;
		void				setIncompleteMessage(const std::string &);
		const std::string	&getIncompleteMessage(void) const;
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
		bool				getUserCheck(void) const;
		void				setUserCheck(void);
		bool				getPassCheck(void) const;
		void				setPassCheck(void);
		void				setEvent();
		struct epoll_event	*getEventAddress(void);
		Server				*getServer(void) const;
		bool				getAskPassword(void) const;
		void				setAskPassword(bool);
		bool				getIsConnected(void) const;
		void				setIsConnected(bool);
		std::set<char>		&getModes(void);
		void				addMode(char);
		void				removeMode(char);
		std::string			getModesString(void);
		// bool				getIsServOperator(void) const;
		// void				setIsServOperator(bool);

		Client &operator=(Client const & rhs);

		bool				isOperator(Channel *);

};

#endif
