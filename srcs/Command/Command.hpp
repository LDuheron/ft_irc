#pragma once
#ifndef COMMAND_HPP
#define COMMAND_HPP

# include "../Server/Server.hpp"

class Server;
class Channel;
class Client;

using std::vector;
using std::string;

class Command
{
	using CommandFunction = std::function<void(Client *, vector<string> &)>;
	
	private:
		// Server	*_server;
		std::map<string, CommandFunction>	_commandMap;
	public:

		Command(Server *server);
		Command(Command const & src);
		~Command();

		Command	&operator=(Command const & rhs);

		void 					registerCommand(const std::string &, CommandFunction);
		static vector<string>	parseCommand(const std::string &);
		static void				handleCommand(Client *client, std::vector<std::string> &);


		static vector<string>	sendCAPLs(Client *client, vector<string> &);
		static vector<string>	sendRPLMessages(Client *client, vector<string> &);
		static vector<string>	sendPASSMessage(Client *client, vector<string> &);
		static vector<string>	checkPassword(Client *client, vector<string> &, const std::string &givenPassword);
		static vector<string>	handleNick(Client *client, vector<string> &);
		static vector<string>	handleUser(Client *client, vector<string> &);
		static vector<string>	handlePing(Client *client, vector<string> &);


		// static void						handleJoin(Client *client, std::string message, std::map<std::string, Channel> _channels);
};

#endif
