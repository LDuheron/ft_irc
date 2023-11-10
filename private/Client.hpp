#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"

class Client
{
	private:
		int			_fd;
		std::string	_nickname;
		std::string	_username;
		// epollfd ??

	public:
		Client();
		~Client();

		int			getFd(void) const;
		void		setFd(int fd);

		Client &	operator=(Client const & rhs);
};

#endif
