#include "Message.hpp"

Message::Message(Client *sender, Client *receiver, const std::string &payload) :
	_sender(sender),
	_receiver(receiver),
	_payload(payload)
{}
