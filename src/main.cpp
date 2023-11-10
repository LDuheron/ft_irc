#include "Server.hpp"

///// generer client via Irssi.org
// commande pour se connecter a irssi :
// /connect localhost <port> <password> <nickname optionnel>
// RAWLOG OPEN debug.log -> voir messages envoyes
// 6667 : standard ports for client-server connections.as port

// CAP LS

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Error: Usage is ./ircserv <port> <password>";
		return (ERROR);
	}
	Server	server(atoi(argv[1]), argv[2]);

	// try 
	// {
		server.init_server();
		server.loop();
	// }
	// catch () {
	// close(this->_socket);

	// }
	// close(server._epollFd);
	return (SUCCESS);
}
