#include "pit.hpp"


Pit::Pit(): Event(){
	this->percept = "You feel a breeze.";
	this->empty = false;
}

void Pit::action(Player& player, int size){
	std::cout << "You wandered into a bottomless pit! You fall to your death!\n";
        player.dead();	
	
}
