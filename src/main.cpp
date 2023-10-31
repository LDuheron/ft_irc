#include "Server.hpp"

///// generer client via Irssi.org
// commande pour se connecter a irssi
// /connect localhost <port> <password> <nickname optionnel>


int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Error: Usage is ./ircserv <port> <password>";
		return (ERROR);
	}
	Server	server(atoi(argv[1]), argv[2]);

	server.init_server();
	// server.loop();
	return (SUCCESS);
}

// bind = connecter serveur a un port
