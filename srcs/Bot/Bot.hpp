#pragma once
#ifndef BOT_HPP
#define BOT_HPP

# include "../Client/Client.hpp"
# include "../Server/Server.hpp"

#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

class Bot
{
	private:
		std::string 				_name;
		std::vector<std::string>	_facts;

		void						biblioFacts(void);

	public:
		Bot();
		~Bot();		
		std::string					getRandomFacts(void);

};

#endif
