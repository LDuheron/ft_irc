#include "Bot.hpp"

// Constructor
Bot::Bot(Server *serv) : _name("bot"), _server(serv), _facts()
{
	this->biblioFacts();
}

// Destructor 

Bot::~Bot()
{
}

// Methods

std::string	Bot::getRandomFacts(void)
{
	int seed = static_cast<int>(time(nullptr));
    srand(seed);
	int randomIndex = rand() % this->_facts.size();
	return (this->_facts[randomIndex]);
}

void	Bot::biblioFacts(void)
{
	this->_facts.push_back("Some paleontologists believe the Spinosaurus might have been semi-aquatic, possessing adaptations similar to modern crocodiles.\n");
	this->_facts.push_back("The Dracorex hogwartsia was named after dragons in Harry Potter due to its fanciful appearance resembling a dragon's head.\n");
	this->_facts.push_back("Climbing has been approved as an Olympic sport, debuting at the 2020 Tokyo Olympics, attracting a global audience and highlighting the sport's competitive nature.\n");
	this->_facts.push_back("The shortest war in history lasted only 38 minutes. It was between Britain and Zanzibar on August 27, 1896.\n");
	this->_facts.push_back("There are more possible iterations of a game of chess than there are atoms in the known universe.\n");
	this->_facts.push_back("Koalas have fingerprints that are almost indistinguishable from human fingerprints.\n");
	this->_facts.push_back("In the 16th century, tulip bulbs were more valuable than gold in Holland.\n");
	this->_facts.push_back("A day on Venus is longer than a year on Venus. Venus rotates so slowly that one day on Venus (a full rotation) takes longer than one orbit around the Sun.\n");
}

void	Bot::sendFacts(Client *client)
{
	std::string msg = this->getRandomFacts();
	_server->sendMessageUser(client, msg);
}
