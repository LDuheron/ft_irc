#include "../Command/Command.hpp"

// Constructor -----------------------------------------------------------------

Command::Command()
{
	if (DEBUG)
		std::cout << "Command default constructor called.";
}

// Destructor ------------------------------------------------------------------

Command::~Command()
{
	if (DEBUG)
		std::cout << "Command destructor called.";
}

// Accessors -------------------------------------------------------------------

// Overload --------------------------------------------------------------------

Command &	Command::operator=(Command const & rhs)
{
	(void)rhs;
	return (*this);
}

// Functions -------------------------------------------------------------------

// void	join(std::string chanName)
// {

// }
