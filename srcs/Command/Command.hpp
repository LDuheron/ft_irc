#pragma once
#ifndef COMMAND_HPP
#define COMMAND_HPP

# include "../Server/Server.hpp"

class Server;
class Channel;
class Client;

class Command
{
	using CommandFunction = std::function<void(Client *)>;
	
	private:
		// Server	*_server;
		std::map<std::string, CommandFunction>	_commandMap;
	public:

		Command(Server *server);
		Command(Command const & src);
		~Command();

		Command &	operator=(Command const & rhs);

		void 							registerCommand(const std::string &command, CommandFunction function);
		static std::vector<std::string>	parseCommand(const std::string &command);
		void							handleCommand(const std::vector<std::string> &parsedCommand);


		static void						sendCAPLs(Client *client);
		static void						sendRPLMessages(Client *client);
		static void						sendPASSMessage(Client *client);
		static void 					checkPassword(Client *client, const std::string &password, const std::string &givenPassword);

		// static void						handleJoin(Client *client, std::string message, std::map<std::string, Channel> _channels);
};

#endif
