#include "room.hpp"
//constructor
Room::Room(int x, int y):symbol(' '){
	this->event = new Event;
	this->position[0] = x;
	this->position[1] = y;
}

//displays the symbol associated with the event in a room
char Room::showSymbol(){
	return this->symbol;
}

//changes the event to gold
void Room::changeGold(){
	delete this->event;
	this->event = new Gold;
	this->symbol = 'G';
}

//changes the event to the wumpus
void Room::changeWumpus(){
	delete this->event;
	this->event = new Wumpus;
	this->symbol = 'W';
}


//changes the event to pit
void Room::changePit(){
	delete this->event;
	this->event = new Pit;
	this->symbol = 'P';
}

//changes the event to bats
void Room::changeBats(){
	delete this->event;
	this->event = new Bats;
	this->symbol = 'B';
}

//display the percept associated with the rooms event
void Room::displayPercept(){
	this->event->showPercept();
	
}

Room::~Room(){

}

//changes the wumpus to a regular event
void Room::shotWumpus(){
	delete this->event;
	this->event = new Event;
}

//exectues an rooms event action
void Room::eventAction(Player& player, int size){
	this->event->action(player,size);
	if(this->symbol == 'G'){
		delete this->event;		//maybe
		this->event = new Event;
		
	}
}
