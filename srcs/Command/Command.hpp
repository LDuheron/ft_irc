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
		vector<vector<string>>				_commandBuffer;

	public:
		Command();
		~Command();

		void 		registerCommand(const string &, CommandFunction);
		void		execCommandBuffered(Client *);
		void		exec(Client *, vector<string> &);
		void		execCmds(Client *, const string &);
		static void	sendPASSMessage(Client *);

		static void	sendCAPLs(Client *, vector<string> &);
		static void	sendRPLMessages(Client *, vector<string> &);
		static void	checkPassword(Client *, vector<string> &);
		static void	handleNick(Client *, vector<string> &);
		static void	handleUser(Client *, vector<string> &);
		static void	handlePing(Client *, vector<string> &);
		static void	handleJoin(Client *client, vector<string> &);
		static void	handlePart(Client *, vector<string> &);
		static void handlePrivmsg(Client *, vector<string> &);
		
		static void handleKick(Client *, vector<string> &);
		static void	handleInvite(Client *client, vector<string> &parsedCommand);

		static void handleBan(Client *, vector<string> &);
		static void handleUnban(Client *, vector<string> &);

		static void	handleOper(Client *client, vector<string> &parsedCommand);

		static void	doNothing(Client *, vector<string> &);
		static void	sendNewLine(Client *);
		static void	shutdown(Client *, vector<string> &);

};

#endif
