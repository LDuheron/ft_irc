#ifndef COMMAND_HPP
#define COMMAND_HPP

# include "../Server/Server.hpp"

class Command
{
	private:

	public:
		Command();
		~Command();

		Command &			operator=(Command const & rhs);

		void	join(std::string chanName);

};

#endif