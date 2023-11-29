#include "Server/Server.hpp"

///// generer client via Irssi.org
// commande pour se connecter a irssi :
// /connect localhost <port> <password> <nickname optionnel>
// RAWLOG OPEN debug.log -> voir messages envoyes
// 6667 : standard ports for client-server connections.as port

// CAP LS

void	signalHandler(int signum)
{
	(void) signum;
	std::cout << "\nExiting server.\n";
	exit(SIGINT);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Error: Usage is ./ircserv <port> <password>";
		return (1);
	}
	Server	server(atoi(argv[1]), argv[2]);
	signal(SIGINT, signalHandler);

	server.init_server();
	std::cout << "Listening on port " << server.getPort() << ".\n";
	while (1)
		server.loop();
	return (0);
}
