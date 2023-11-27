#pragma once
#ifndef MESSAGE_H
# define MESSAGE_H

# include "../Server/Server.hpp"


class Message
{
public:

	Message();
	Message(const Message &toCopy);
	~Message();

	Message	&operator=(const Message &rhs);

protected:

	Client *_sender;
	Client *_receiver;
	std::string _payload;

};
#endif
