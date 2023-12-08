#pragma once
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "../Server/Server.hpp"

# define REGULAR 0
# define LOCAL 1
// # define INVITATION_ONLY 1
// # define PRIVATE 2

# define FALSE 0
# define TRUE 1

// histoire du buffer avec netcat pour les messages.
class Server;
class Client;

class Channel
{
	private:
		std::vector<Client *>		_members;
		std::vector<Client *>		_operator;
		std::vector<Client *>		_invited;

		bool						_hasPassword;
		int							_cptUser;
		int							_maxUser;
		std::string					_name;
		std::string					_password;
		std::string					_topic; // general description of the channel
		int							_type; // regular #, local &

	// mutable topic ?

	public:
		Channel();
		Channel(Channel const & src);
		~Channel();

		std::vector<Client *> const &getMembers() const;

		const int 			&getCptUser(void);

		const int 			&getMaxUser(void);
		void				setMaxUser(int const &maxUser);

		const std::string	&getName(void);
		void				setName(std::string const &name);

		// const std::string	&getPassword(void);
		void				setPassword(std::string const &password);

		const std::string	&getTopic(void);
		void				setTopic(std::string const &name);

		const int 			&getType(void);
		void				setType(int const &type);

		Channel &			operator=(Channel const & rhs);

		void				addMember(Client *client);
		void				removeMember(Client *client);
		void				removeMember(std::string client);

		void				addOperator(Client *client);
		void				removeOperator(Client *client);

		void				inviteMember(Client *client);
		void				uninviteMember(Client *client);

		int					isMember(Client *client);
		int					isOperator(Client *client);
		int					isInvited(Client *client);

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
