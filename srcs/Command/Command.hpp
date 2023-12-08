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
		std::map<string, CommandFunction>	_commandMap;

	public:
		Command();
		~Command();

		void 							registerCommand(const string &, CommandFunction);
		static vector<vector<string>>	parseLine(Client *, const string &);
		// static vector<string>			parseCommand(vector<string> &);
		void							handleCommand(Client *client, const string &);
		static void				sendPASSMessage(Client *);

		static void				sendCAPLs(Client *, vector<string> &);
		static void				sendRPLMessages(Client *, vector<string> &);
		static void				checkPassword(Client *, vector<string> &);
		static void				handleNick(Client *, vector<string> &);
		static void				handleUser(Client *, vector<string> &);
		static void				handlePing(Client *, vector<string> &);
		static void				doNothing(Client *, vector<string> &);
		static void				sendNewLine(Client *);

		static void				handlePrivmsg(Client *client, vector<string> &parsedCommand);
		static void				handleKick(Client *client, vector<string> &parsedCommand);

		static void				handleJoin(Client *client, vector<string> &parsedCommand);
};

#endif
