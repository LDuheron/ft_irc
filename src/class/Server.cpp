
#include "Server.hpp"

// Constructor -----------------------------------------------------------------

Server::Server() :
	_epollFd(0), _epollEvent(), _IP(0),
	_nickname("DEFAULT"), _nbClients(0), _password("NULL"),
	_port(0), _serverAddr(), _socket(0)
{
	if (DEBUG)
		std::cout << "Server : default constructor called.\n";
}

Server::Server(int port, std::string password) :
	_epollFd(0), _epollEvent(), _IP(0),
	_nickname("DEFAULT"), _nbClients(0), _password(password),
	_port(port), _serverAddr(), _socket(0)
{
	if (DEBUG)
		std::cout << "Server : Param port and password constructor called.\n";
}

Server::Server(Server const & src) :
	_epollFd(0), _epollEvent(), _IP(src._IP),
	_nickname(src._nickname), _nbClients(src._nbClients), _password(src._password),
	_port(src._port), _serverAddr(), _socket(src._socket)
	
{
	if (DEBUG)
		std::cout << "Server : copy constructor called.\n";
}

// Destructor ------------------------------------------------------------------

Server::~Server()
{
	if (DEBUG)
		std::cout << "Server : destructor called.\n";
}

// Accessors -------------------------------------------------------------------

std::string const &	Server::getNickname(void) const
{
	return (this->_nickname);
}

std::string const &	Server::getPassword(void) const
{
	return (this->_password);
}

int const &	Server::getIP(void) const
{
	return (this->_IP);
}

int const &	Server::getPort(void) const
{
	return (this->_port);
}

void	Server::setSocket(int newSocket)
{
	this->_socket = newSocket;
}

// Overload --------------------------------------------------------------------

Server &	Server::operator=(Server const & rhs)
{
	this->_IP = rhs._IP;
	this->_nickname = rhs._nickname;
	this->_password = rhs._password;
	this->_port = rhs._port;
	this->_socket = rhs._socket;
	this->_serverAddr = rhs._serverAddr;
	return *this;
}

std::ostream & operator<<(std::ostream & lhs, Server const & rhs)
{
	lhs << "IP : " << rhs.getIP()
		<< "\nNickname : " << rhs.getNickname()
		<< "\nPassword : " << rhs.getPassword()
		<< "\nPort : " << rhs.getPort()
		<< "\nSocket : Unavailable"
		<< ".\n";
	return lhs;
}

// Functions - init data -------------------------------------------------------------------

// https://ncona.com/2019/04/building-a-simple-server-with-cpp/

// INIT_SERVERADDR : This function initializes the structure of data sockaddr_in.
// sin_family is AF_INET = gives to socket an IPv4 socket address to allow it to
// communicate with other hosts over a TCP/IP network.
// sin_port member defines the TCP/IP port number for the socket address.
void	Server::init_serverAddr(void)
{
	this->_serverAddr.sin_family = AF_INET;
	this->_serverAddr.sin_port = htons(this->_port);
	this->_serverAddr.sin_addr.s_addr = INADDR_ANY;
}

void	Server::init_server(void)
{
	init_serverAddr();
	this->_epollFd = epoll_create1(0);
	if (this->_epollFd == FAIL)
		std::cerr << "Error: Failed to epoll.\n";

	setSocket(socket(AF_INET, SOCK_STREAM, 0));
	if (this->_socket == FAIL)
		std::cerr << "Error : Failed to create socket.\n";

	// setsocktopt() ???

	if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) == FAIL)
		std::cerr << "Error: Failed to configurate fd in O_NONBLOCK mode.\n";

	if (bind(this->_socket, (sockaddr*)&(this->_serverAddr), sizeof(this->_serverAddr)) == FAIL)
		std::cerr << "Error : Failed to bind to port " << this->_port << ".\n";

	if (listen(this->_socket, MAX_CLIENTS) == FAIL)
		std::cerr << "Error : Failed to listen.\n";

	if (DEBUG)
		std::cout << "Server initialisation successful.\n";

	// if (epoll_wait(this->_epollFd, &this->_epollEvent, MAX_CLIENTS, -1) == FAIL)
	// 	std::cerr << "Error : Epoll_wait() failed.\n";

	// this->_allSockets.push_back(this->_socket);

	// RPL_WELCOME message

	// close(this->_epollFd);
}

// Functions - launch server -------------------------------------------------------------------

void	Server::check_inactivity(void)
{
// 	send() send ping wait for pong?????????????
}

void	Server::handleNewClient(void)
{
	Client *newClient;
	// // memset newClient?;
	
	// if (accept(this->_socket, (sockaddr*)&(this->_serverAddr), sizeof(this->_serverAddr)) == FAIL)
	// 	std::cerr << "Error : Failed to accept.\n";

	// if (this->_nbClients + 1 > MAX_CLIENTS)
	// {
	// 	send(); message too much clients
	// 	close(client.getFd());
	// 	return ;
	// }

// in success
	
	// send(":IRC 001 ", this->_nickname, ":Welcome to the IRC Network, ", this->_nickname, "\n");
	newClient = new Client;
	(*newClient).setFd(socket(AF_INET, SOCK_STREAM, 0));
	if ((*newClient).getFd() == FAIL)
		std::cerr << "Error : Failed to create socket.\n";
	if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) == FAIL)
		std::cerr << "Error: Failed to configurate fd in O_NONBLOCK mode.\n";
	this->_allClients.push_back(newClient);
	this->_nbClients += 1;
}

void	Server::handleNewRequest(void)
{
	// recv ???????????????

	// if (bind(this->_socket, (sockaddr*)&(this->_serverAddr), sizeof(this->_serverAddr)) == FAIL)
	// 	std::cerr << "Error : Failed to bind to port " << this->_port << ".\n";

	// if (listen(this->_socket, MAX_CLIENTS) == FAIL)
	// 	std::cerr << "Error : Failed to listen.\n";

	// if (accept(this->_socket, (sockaddr*)&(this->_serverAddr), sizeof(this->_serverAddr)) == FAIL)
	// 	std::cerr << "Error : Failed to accept.\n";
}

	// if (connect(this->_socket, (sockaddr*)&(this->_serverAddr), sizeof(this->_serverAddr)) == FAIL)
	// 	std::cerr << "Error : Failed to connect.\n";

void	Server::loop(void)
{
	check_inactivity();
	if (DEBUG)
		std::cout << "Enter in server loop.\n"; 
	// if (epoll()) -> iterate on all fds stored in a vector
	// {
	// }
	// if ()
	handleNewClient();
	// else
	handleNewRequest();
}
