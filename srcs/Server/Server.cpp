#include "../Server/Server.hpp"

using std::string;
using std::vector;

// Constructor -----------------------------------------------------------------

Server::Server(int port, std::string password) :
	_IP(0),
	_epollSocket(0),
	_nbClients(0),
	_serverPort(port),
	_serverSocket(0),
	_serverName("DEFAULT"),
	_serverPassword(password),
	_serverEvent(),
	_serverAddr(),
	_clientMap(),
	_command(new Command())
{}

// Destructor ------------------------------------------------------------------

Server::~Server()
{
	for (std::map<int, Client *>::iterator it = this->_clientMap.begin(); it != this->_clientMap.end(); ++it)
		delete it->second;
}

// Accessors -------------------------------------------------------------------

std::string const				&Server::getNickname(void) const { return (this->_serverName); }

std::string const				&Server::getPassword(void) const { return (this->_serverPassword); }

int const						&Server::getIP(void) const { return (this->_IP); }

int const						&Server::getPort(void) const { return (this->_serverPort); }

void							Server::setSocket(int newSocket) { this->_serverSocket = newSocket; }

std::map<int, Client *> const	&Server::getClientMap(void) const { return (this->_clientMap); }

void		Server::start(void)
{
	init_server();
	while (1)
		loop();
}

void		Server::init_serverAddr(void)
{
	memset(&this->_serverAddr, 0, sizeof(this->_serverAddr));
	this->_serverAddr.sin_family = AF_INET;
	this->_serverAddr.sin_port = htons(this->_serverPort);
	this->_serverAddr.sin_addr.s_addr = INADDR_ANY;
}

static int	createSocket(void)
{
	int newSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (newSocket == -1)
	{
		std::perror("Error: Failed to create socket");
		std::exit(-1);
	}
	return (newSocket);
}

static int	createEpoll(void)
{
	int newEpoll = epoll_create1(0);
	if (newEpoll == -1)
	{
		std::perror("Error: Failed to epoll");
		std::exit(-1);
	}
	return (newEpoll);
}

static void	controllEpoll(int epollSocket, int operation, int socket, struct epoll_event *event)
{
	if (epoll_ctl(epollSocket, operation, socket, event) == -1)
	{
		std::perror("Error: Failed to control epoll");
		std::exit(-1);
	}
}

static void	bindSocket(int socket, const struct sockaddr *addr)
{
	if (bind(socket, (struct sockaddr *)addr, sizeof(*addr)) == -1)
	{
		std::perror("Error: Failed to bind socket");
		std::exit(-1);
	}
}

static void	listenSocket(int socket)
{
	if (listen(socket, MAX_CLIENTS) == -1)
	{
		std::perror("Error: Failed to listen socket");
		std::exit(-1);
	}
}

void		Server::init_server(void)
{
	init_serverAddr();

	// Create socket
	setSocket(createSocket());
	if (DEBUG2)
		std::cout << "===== server socket : " << this->_serverSocket << " =====" << std::endl;

	//epoll call
	this->_epollSocket = createEpoll();
	this->_serverEvent.events = EPOLLIN ; // EPOLLET for edge-triggered events ?
	this->_serverEvent.data.fd = this->_serverSocket;
	controllEpoll(this->_epollSocket, EPOLL_CTL_ADD, this->_serverSocket, &this->_serverEvent);

	// Bind socket to IP / port
	bindSocket(this->_serverSocket, (sockaddr *)&(this->_serverAddr));

	// Listen on socket
	listenSocket(this->_serverSocket);
}

// Functions - launch server -------------------------------------------------------------------

static int	acceptNewClient(int serverSocket, struct sockaddr_in serverAddr)
{
	int			clientSocket;
	socklen_t	addrLen = sizeof(serverAddr);

	if ((clientSocket = accept(serverSocket, (struct sockaddr*)&(serverAddr), &addrLen)) == -1)
	{
		std::perror("Error : Failed to accept client connection.\n");
		close(clientSocket);
		return -1;
	}
	return (clientSocket);
}

static void	controlSocket(int socket, int operation)
{
	if (fcntl(socket, F_SETFL, operation) == -1)
	{
		std::perror("Error: Failed to control socket");
		close(socket);
	}
}

static Client	*getClient(int clientSocket, Server *server, std::map<int, Client *> &clientMap, int &nbClients)
{
	if (nbClients >= MAX_CLIENTS)
	{
		std::cerr << "Error : Too much clients connected.\n";
		close(clientSocket);
		return NULL;
	}

	Client *newClient = new Client(clientSocket, server);
	clientMap.insert(std::make_pair(newClient->getSocket(), newClient));
	++nbClients;
	return newClient;
}

void		Server::handleNewClient(void)
{
	int			clientSocket;

	clientSocket = acceptNewClient(this->_serverSocket, this->_serverAddr);
	if (clientSocket == -1)
		return ;
	if (DEBUG2)
		std::cout << "===== Client socket : " << clientSocket << " =====" << std::endl;
	
	// Set client socket to non-blocking mode
	controlSocket(clientSocket, O_NONBLOCK);

	Client *newClient = getClient(clientSocket, this, this->_clientMap, this->_nbClients);

	controllEpoll(this->_epollSocket, EPOLL_CTL_ADD, clientSocket, newClient->getEventAddress());
}

void		Server::loop(void)
{
	int					fd_ready;
	struct epoll_event	events[MAX_CLIENTS];

	if ((fd_ready = epoll_wait(this->_epollSocket, events, MAX_CLIENTS, -1)) == -1)
		std::perror("Error: Failed to wait for events");

	for (int i = 0; i < fd_ready; ++i)
	{
		int currentSocket = events[i].data.fd;
		if (currentSocket == this->_serverSocket)
			handleNewClient();
		else
			handleClientEvent(this->_clientMap[currentSocket]);
	}
}

void		Server::removeClient(Client *client)
{
	epoll_ctl(this->_epollSocket, EPOLL_CTL_DEL, client->getSocket(), client->getEventAddress());
	this->_clientMap.erase(client->getSocket()); // make fct removeClient
	delete client;
	this->_nbClients--;
}

void		Server::handleClientError(Client *client)
{
	if (errno == EAGAIN || errno == EWOULDBLOCK) // no data currently available, doesn't necessarily mean an error occured
		return;
	else
	{
		std::perror("Error : Failed to receive data");
		removeClient(client);
	}
}

void		Server::handleClientDisconnection(Client *client)
{
	std::cout << "Client disconnected: " << client->getSocket() << std::endl;
	removeClient(client);
}

void		Server::handleClientEvent(Client *client)
{
	char	buffer[MAX_MESSAGE_LENGTH];
	int		bytesRead;

	bytesRead = recv(client->getSocket(), buffer, MAX_MESSAGE_LENGTH, 0);
	if (bytesRead < 0)
		handleClientError(client);
	else if (bytesRead == 0)
		handleClientDisconnection(client);
	else
	{
		buffer[bytesRead] = '\0';
		processIncomingData(client, buffer);
	}
}

// Remove this function and put evertything in handleCommand ?
void		Server::processIncomingData(Client *client, const std::string message)
{
	// vector<string>	parsedLines = Command::parseLine(message);
	// vector<string>	parsedCommand = Command::parseCommand(parsedLines);
	this->_command->handleCommand(client, message);
	
	// if (message.substr(0,6) == "CAP LS")
	// 	Command::sendCAPLs(client);
	// else if (message.substr(0,7) == "CAP END")
	// 	Command::sendPASSMessage(client, parsedLines);
	// else if (message.substr(0,4) == "PASS")
	// 	Command::checkPassword(client, parsedLines);
	// else if (message.substr(0, 4) == "PING")
	// 	Command::handlePing(client, parsedLines);
	// else if (message.substr(0, 4) == "JOIN")
	// 	Command::join(client, message, this->_channels);
	/*
	else if (msg.substr(0, 4) == "KICK")
	else if (msg.substr(0, 6) == "INVITE")
	else if (msg.substr(0, 5) == "TOPIC")
	else if (msg.substr(0, 4) == "MODE")
	*/
}
