#include "event.hpp"

//constructor
Event::Event(): percept(""), empty(true), symbol(0){}

//standard event action is to report moving to a new room
void Event::action(Player& player, int size){
	
	std::cout << "\nYou have wandered into dark empty room\n";  
}

//shows the percept of the event 
void Event::showPercept(){
	std::cout << percept << std::endl;
}

Event::~Event(){
	
}
