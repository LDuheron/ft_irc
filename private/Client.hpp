#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"

class Client
{
	private:
		int			_fd;
		std::string	_nickname;
		// epollfd

	public:
		Client();
		~Client();
		void		setFd(int fd);
		Client &	operator=(Client const & rhs);
};

#endif
