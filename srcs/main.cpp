#include "Server/Server.hpp"

bool shutdown_signal = false;

void	signalHandler(int signum)
{
	(void) signum;
	shutdown_signal = true;
	std::cout << "\nExit signal received.\n";
}

void	checkArguments(char *port, char *pass)
{
	for (int i = 0; port[i]; i++)
	{
		if (!isdigit(port[i]))
		{
			std::cerr << "Error: port can only contain digits.\n";
			exit(1);
		}
	}

	int port_int = atoi(port);
	if (port_int < 0 || port_int > 65535)
	{
		std::cerr << "Error: port should be in range [0;65535].\n";
		exit(1);
	}
	if (!pass[0])
	{
		std::cerr << "Error: password cannot be empty.\n";
		exit(1);
	}
}

int main(int argc, char **argv)
{
	shutdown_signal = false;
	if (argc != 3)
	{
		std::cerr << "Error: Usage is ./ircserv <port> <password>\n";
		return (1);
	}
	checkArguments(argv[1], argv[2]);
	Server	server(atoi(argv[1]), argv[2]);
	signal(SIGINT, signalHandler);
	server.start();
	return (0);
}
