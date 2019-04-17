#include "gold.hpp"

//constructor
Gold::Gold(): Event(){
	this->percept = "You see a glimmer nearby.";
	this->empty = false;
}

//display action message and run found gold function for the player
void Gold::action(Player& player, int size){
	std::cout << "You have found the Wumpus' gold!\n";
	player.foundGold();
}
