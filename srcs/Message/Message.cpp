#include "Message.hpp"

Message::Message()
{}

Message::Message(const Message &toCopy)
{
	*this = toCopy;
}

Message::~Message()
{}

Message	&Message::operator=(const Message &rhs)
{
	if (this == &rhs)
		return *this;
	return *this;
}
