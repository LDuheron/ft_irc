#pragma once
#ifndef MESSAGE_H
# define MESSAGE_H

# include "../Server/Server.hpp"

class Message
{
	public:

		Message(Client *, Client *, const std::string &);
		~Message();

	protected:

		Client *_sender;
		Client *_receiver;
		std::string _payload;

};
#endif
