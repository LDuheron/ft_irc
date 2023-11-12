#pragma once
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include "Server.hpp"

class Channel
{
	private:
		std::string	_name;
		std::string	_password;

	public:
		std::string	admin;
		std::string	nbClientConnected;
		// banned  ?
		// allowed ?

};

// The channel is created implicitly when the first
// client joins it, and the channel ceases to exist
// when the last client leaves it. 

#endif
