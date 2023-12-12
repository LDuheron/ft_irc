#pragma once
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
		int						_maxUser;
		string					_password;
		string					_topic; // general description of the channel

	// mutable topic ?

	public:
		Channel(const string &name);
		// Channel(Channel const & src);
		~Channel();

		const string	&getName(void) const;
		// void			setName(string const &name);

		const int 		&getMaxUser(void);
		void			setMaxUser(int const &maxUser);

		// const string	&getPassword(void);
		void			setPassword(string const &password);

		const string	&getTopic(void);
		void			setTopic(string const &name);

		Channel &			operator=(Channel const & rhs);

		void			banMember(Client *client);
		void			unbanMember(Client *client);

		void			addMember(Client *client);
		void			removeMember(Client *client);

		void			addOperator(Client *client);
		void			removeOperator(Client *client);

		void			inviteMember(Client *client);
		void			uninviteMember(Client *client);

		int				isOperator(Client *client);
		int				isInvited(Client *client);

		void			listUsers();
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
