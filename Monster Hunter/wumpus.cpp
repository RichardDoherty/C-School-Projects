#include "wumpus.hpp"

Wumpus::Wumpus(): Event(){
	this->percept = "You smell a terrible stentch.";
	this->empty = false;

}

void Wumpus::action(Player& player, int size){
	
	std::cout << "You have wandered into the Wumpus' lair!\nYou have have been eaten alive!";
	player.dead();
}
