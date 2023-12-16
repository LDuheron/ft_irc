#pragma once
#ifndef BOT_HPP
#define BOT_HPP

# include "../Client/Client.hpp"
# include "../Server/Server.hpp"

#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

class Client;
class Server;

class Bot
{
	private:
		std::string 				_name;
		Server 						*_server;
		std::vector<std::string>	_facts;

		void		biblioFacts(void);
		std::string	getRandomFacts(void);

	public:
		Bot(Server *serv);
		~Bot();
		void	sendFacts(Client *);
	
};

#endif
