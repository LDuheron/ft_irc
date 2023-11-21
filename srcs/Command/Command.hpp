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

		static void		handleJoin(Client *client, std::string message, std::map<std::string, Channel> _channels);
		/*Gestion temporaire du ping

		@param clientSocket : socket du client qui a envoyé le ping
		*/
		static void		handlePing(int clientSocket, const std::string &pingData);
		static void		handlePrivmsg(Channel _all);
};

#endif
