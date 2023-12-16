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
	this->_facts.push_back("There are more possible iterations of a game of chess than there are atoms in the known universe.\n");
	this->_facts.push_back("Koalas have fingerprints that are almost indistinguishable from human fingerprints.\n");
	this->_facts.push_back("In the 16th century, tulip bulbs were more valuable than gold in Holland.\n");
	this->_facts.push_back("A day on Venus is longer than a year on Venus. Venus rotates so slowly that one day on Venus (a full rotation) takes longer than one orbit around the Sun.\n");
	this->_facts.push_back("Honey never spoils. Archaeologists have found pots of honey in ancient Egyptian tombs that are over 3,000 years old and still perfectly edible.\n");
	this->_facts.push_back("The original name for Bank of America was 'Bank of Italy.'\n");
	this->_facts.push_back("The shortest war in history was between Britain and Zanzibar on August 27, 1896. Zanzibar surrendered after 38 minutes.\n");
	this->_facts.push_back("There are more possible iterations of a deck of cards than there are atoms on Earth.\n");
	this->_facts.push_back("A 'jiffy' is an actual unit of time: 1/100th of a second.\n");
	this->_facts.push_back("The dot over the letters 'i' and 'j' is called a 'tittle.'\n");
	this->_facts.push_back("Bananas are berries, but strawberries aren't.\n");
	this->_facts.push_back("The inventor of the frisbee was turned into a frisbee. Walter Morrison, the inventor, was cremated, and his ashes were turned into a limited-edition frisbee after he passed away.\n");
	this->_facts.push_back("In 1386, a pig in France was executed by public hanging for the murder of a child.\n");
	this->_facts.push_back("Wombat poop is cube-shaped.\n");
	this->_facts.push_back("The word 'nerd' was first coined by Dr. Seuss in 'If I Ran the Zoo' in 1950.\n");
	this->_facts.push_back("Cows have best friends.\n");
	this->_facts.push_back("The Great Wall of China is not visible from the moon without aid.\n");
	this->_facts.push_back("The world's largest desert is not the Sahara; it's Antarctica.\n");
	this->_facts.push_back("Octopuses have three hearts.\n");
	this->_facts.push_back("The Eiffel Tower can be 15 cm taller during the summer due to thermal expansion.\n");
	this->_facts.push_back("The average person will spend six months of their life waiting for red lights to turn green.\n");
	this->_facts.push_back("The oldest known vegetable is the pea.\n");
	this->_facts.push_back("A group of flamingos is called a 'flamboyance.'\n");
	this->_facts.push_back("Starfish do not have a brain.\n");
	this->_facts.push_back("The longest English word without a vowel is 'rhythms.'\n");
	this->_facts.push_back("The longest war between two nations lasted 781 years between England and France, known as the Hundred Years' War.\n");
	this->_facts.push_back("The average person will walk around 70,000 miles in their lifetime.\n");
	this->_facts.push_back("A crocodile cannot stick its tongue out.\n");
	this->_facts.push_back("Starfish do not have a brain.\n");
}

void	Bot::sendFacts(Client *client)
{
	std::string msg = this->getRandomFacts();
	_server->sendMessageUser(client, msg);
}
