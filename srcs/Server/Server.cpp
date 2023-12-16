#include "../Server/Server.hpp"
#include <map>

using std::string;
using std::vector;

// Constructor -----------------------------------------------------------------

Server::Server(int port, std::string password) :
	_IP(0),
	_epollSocket(0),
	_nbClients(0),
	_serverPort(port),
	_serverSocket(0),
	_serverName("ircserv"),
	_serverPassword(password),
	_serverEvent(),
	_serverAddr(),
	_serverOperator(),
	_clientMap(),
	_clientMapStr(),
	_channelMap(),
	_command(new Command()),
	_shutdown(false)
{}

// Destructor ------------------------------------------------------------------

Server::~Server()
{
	for (std::map<int, Client *>::iterator it = this->_clientMap.begin(); it != this->_clientMap.end(); ++it)
		delete it->second;
	this->_clientMap.clear();
	this->_clientMapStr.clear();
	for (std::map<string, Channel *>::iterator it = this->_channelMap.begin(); it != this->_channelMap.end(); ++it)
		delete it->second;
	this->_channelMap.clear();
	delete this->_command;
}

// Accessors -------------------------------------------------------------------

std::string const			&Server::getNickname(void) const { return (this->_serverName); }

std::string const			&Server::getPassword(void) const { return (this->_serverPassword); }

int const					&Server::getIP(void) const { return (this->_IP); }

int const					&Server::getPort(void) const { return (this->_serverPort); }

void						Server::setSocket(int newSocket) { this->_serverSocket = newSocket; }

std::map<string, Client *> 	&Server::getServerOperator(void) { return (this->_serverOperator); }

std::map<int, Client *> 	&Server::getClientMap(void) { return (this->_clientMap); }

std::map<string, Client *> 	&Server::getClientMapStr(void) { return (this->_clientMapStr); }

std::map<string, Channel *>	&Server::getChannelMap(void) { return (this->_channelMap); }

void						Server::setShutdown(bool shutdown) { this->_shutdown = shutdown; }

// Functions - init server -------------------------------------------------------------------
void		Server::start(void)
{
	init_server();
	while (this->_shutdown == false)
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
	this->_clientMapStr.insert(std::make_pair(newClient->getNickname(), newClient));

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
	this->_clientMap.erase(client->getSocket());
	this->_clientMapStr.erase(client->getNickname());
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
		this->_command->execCmds(client, buffer);
		// this->_command->handleCommand(client, buffer);
		// processIncomingData(client, buffer);
	}
}

void			Server::sendMessage(Client *client, string &message)
{
	string 	server = ":" + client->getServer()->getNickname();
	message = server + " " + message + "\r\n";
	if (send(client->getSocket(), message.c_str(), message.length(), 0) == -1)
		std::perror("send:");
	std::cout << "\n----- Server response -----\n";
	std::cout << message;
	std::cout << "---------------------------\n";
}

void			Server::sendMessageUser(Client *client, string &message)
{
	string 	nick = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname();
	message = nick + " " + message + "\r\n";
	if (send(client->getSocket(), message.c_str(), message.length(), 0) == -1)
		std::perror("send:");
	std::cout << "\n----- Server response (user) -----\n";
	std::cout << message;
	std::cout << "---------------------------\n";
}

// pas le temps d'être clean j'ai transcendance à taffer
void			Server::sendMessageRaw(Client *client, string &message)
{
	if (send(client->getSocket(), message.c_str(), message.length(), 0) == -1)
		std::perror("send:");
	std::cout << "\n----- Server response (raw) -----\n";
	std::cout << message;
	std::cout << "---------------------------\n";
}

void			Server::sendMessageChannel(Channel *channel, string &message, Client *client)
{
	for (vector<Client *>::const_iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); ++it)
	{
		string msg = ":" + (*it)->getNickname() + "!" + (*it)->getUsername() + "@" + (*it)->getHostname() + " " + message + "\r\n";
		if ( *it != client)
			sendMessage(*it, message);
	}
	std::cout << "\n----- Server response (channel) -----\n";
	std::cout << message;
	std::cout << "---------------------------\n";
}


void		Server::addServOperator(Client *newOperator, std::string password)
{
	if (this->getPassword() == password)
		this->_serverOperator.insert(std::make_pair(newOperator->getNickname(), newOperator));
	// else
	// {
		
	// }
}

void		Server::removeServOperator(Client * client)
{
	this->_serverOperator.erase(client->getNickname());
}
