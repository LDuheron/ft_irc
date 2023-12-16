#pragma once
#include <set>
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "../Server/Server.hpp"

# define REGULAR 0
// # define INVITATION_ONLY 1
// # define PRIVATE 2

# define FALSE 0
# define TRUE 1

// histoire du buffer avec netcat pour les messages.
class Server;
class Client;

using std::vector;
using std::string;
class Channel
{
	private:
		string					_name;
		vector<Client *>		_banned;
		vector<Client *>		_members;
		vector<Client *>		_operator;
		vector<Client *>		_invited;
		bool					_hasPassword;
		bool					_inviteOnly;
		bool					_hasUserLimit;
		bool					_hasTopicProtection;
		string					_topic;
		Server 					*_server;
	
		std::set<char>			_modes; // modes of the channel
		int						_cptUser;
		int						_maxUser;
		string					_password;
		int						_userLimit;

	public:
		Channel(const string &name, Server *server);
		~Channel();

		const string	&getName(void) const;

		vector<Client *> const	&getMembers(void) const;
		vector<Client *> const	&getOperators(void) const;

		const string	&getTopic(void);
		void			setTopic(string const &name);

		Server 			*getServer(void) const;

		Channel &			operator=(Channel const & rhs);
		vector<Client *> const	&getInvited(void) const;

		const int 		&getMaxUser(void);
		void			setMaxUser(int const &maxUser);

		// const string	&getPassword(void);
		void			setPassword(string const &password);


		void			banMember(Client *client);
		void			unbanMember(Client *client);

		void			addMember(Client *client);
		void			removeMember(Client *client);
		bool			isMember(Client *client);
		void			removeMember(std::string client);


		void			addOperator(Client *client);
		void			removeOperator(Client *client);
		bool			isOperator(Client *client);

		void			setInviteOnly(bool inviteOnly);
		bool			getInviteOnly(void) const;
		void			inviteMember(Client *client);
		void			uninviteMember(Client *client);
		bool			isInvited(Client *client);

		bool			isBan(Client *client);

		void			listUsers();
	

		std::set<char>		&getModes(void);
		void				addMode(char);
		void				removeMode(char);
		std::string			getModesString(void);

		void 			setHasPassword(bool hasPassword);
		bool 			getHasPassword(void) const;
		bool 			checkPassword(const string &) const;


		void 			setHasUserLimit(bool hasUserLimit);
		bool 			getHasUserLimit(void) const;
		void 			setUserLimit(int limit);
		// void			unsetUserLimit(void);
		int 			getUserLimit(void) const;

		void 			setTopicProtection(bool hasTopicProtection);
		bool 			getTopicProtection(void) const;

		
};

/*
important info form doc

- channel name may not contain any spaces " "
the first client who joins a normal channel is made a channel operator


Channel ownership ? 
Local Channel 
*/
// The channel is created implicitly when the first
// client joins it, and the channel ceases to exist
// when the last client leaves it. 

#endif
