#include "bats.hpp"

//constructor
Bats::Bats(): Event(){
	this->percept = "You hear wings flapping.";
	this->empty = false;
}

//display bat action message and then move the user to a random loaction in the vector
void Bats::action(Player& player, int size){
	
	std::cout << "You wandered into a room infested with giant bats!\n";
	
	//seed time for a truely random value
	srand(time(NULL));
	int x = rand() % size;
	int y = rand() % size;
	player.enter(x,y);

}
