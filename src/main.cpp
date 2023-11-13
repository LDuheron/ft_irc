#include "../private/Server.hpp"

///// generer client via Irssi.org
// commande pour se connecter a irssi :
// /connect localhost <port> <password> <nickname optionnel>
// RAWLOG OPEN debug.log -> voir messages envoyes
// 6667 : standard ports for client-server connections.as port

// CAP LS

void	signalHandler(int signum)
{
	(void) signum;
	exit(SIGINT);
	// close fd ?
}

// CLIENT OPERATOR ????????????????

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Error: Usage is ./ircserv <port> <password>";
		return (EXIT_FAILURE);
	}
	signal(SIGINT, signalHandler);
	Server	server(atoi(argv[1]), argv[2]);

	// try catch  ?
	server.init_server();
	std::cout << "Listening on port " << server.getPort() << ".\n";
	while (1)
	{
		server.loop();
		// for (size_t i = 0; i < server.getAllClients().size(); i++)
		// {
		server.processMessages();
		// }
	}
	// close(server._epollFd);
	return (EXIT_SUCCESS);
}
