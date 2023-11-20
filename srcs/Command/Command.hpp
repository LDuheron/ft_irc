#ifndef COMMAND_HPP
#define COMMAND_HPP

# include "../Server/Server.hpp"

class Channel;
class Server;
class Message;
class Client;

class Command
{
	private:

	public:
		Command();
		Command(Command const & src);
		~Command();

		Command &	operator=(Command const & rhs);

		static void		join(Client *client, std::string message, std::map<std::string, Channel> _channels);

};

#endif