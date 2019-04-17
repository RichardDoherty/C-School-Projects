#include "game.hpp"

//creates player with inital position equal to escape rope and initalizes private game members
Game::Game(int size):size(size){

	//randomly select a position for the escape rope and inital user location
	srand(time(NULL));						
	for(int n = 0; n < 2; n++)
		this->exitRope[n] = rand() % size;

	this->player.enter(this->exitRope[0],this->exitRope[1]);

	//craete the grid array
	for(int row = 0; row < size; row++){

		for(int col = 0; col < size; col++){
			cave.push_back(Room(row,col));		
		}	
	}
}

//this function will allow the user to attempt to shoot at the wumpus every turn
void Game::shootArrow(){
	
	//get the vector indexed position of the player
	int* position = this->player.currentPosition();
	int vecPosition = (position[1]*this->size + position[0]);
	
	//prompt the user to shoot an arrow
	std::cout << "Arrow: " << this->player.numArrows() << "\nWould you like to fire an arrow at the Wumpus?\nEnter yes to fire: ";
	std::string input;
	std::cin >> input;
	
	//if the user answers yes and has arrows then determine the direction to shoot in
	if (input == "yes" && this->player.numArrows() > 0){
		do{
			std::cout << "What direction would you like to fire your arrow?\nEnter up, down, left, or right: " ;
			std::cin >> input;
		} while(input != "up" && input != "down" && input != "left" && input != "right");
		
		//lose an arrow
		this->player.loseArrow();
		
		//check if the arrow hits the wumpus depending on the direction the arrow is fired
		if(input == "up"){
			for (int n = 0; n < 3; n++){
				if((vecPosition - (n+1)*this->size) >= 0 ){
					if(cave[(vecPosition - (n+1)*this->size)].showSymbol() == 'W'){
						std::cout << "You shot the Wumpus!\n";
						cave[(vecPosition - (n+1)*this->size)].shotWumpus();
						return;
					}
				}
			}
		}
		
		//if the function reaches this point the user has missed the wumpus
		//now we determine if the wumpus has heard the shot and if he moves to a new location
		if(input == "down"){
			for (int n = 0; n < 3; n++){
				if((vecPosition + (n+1)*this->size) >= 0 ){
					if(cave[(vecPosition + (n+1)*this->size)].showSymbol() == 'W'){
						std::cout << "You shot the Wumpus!\n";
						cave[(vecPosition + (n+1)*this->size)].shotWumpus();
						return;
					}
				}
			}
		}
	
		if(input == "left"){
			for (int n = 0; n < 3; n++){
				if((vecPosition - (n+1)) >= 0 ){
					if(cave[vecPosition - (n+1)].showSymbol() == 'W'){
						std::cout << "You shot the Wumpus!\n";
						cave[vecPosition - (n+1)].shotWumpus();
						return;
					}
				}
			}
		}

		if(input == "right"){
			for (int n = 0; n < 3; n++){
				if((vecPosition + (n+1)) >= 0 ){
					if(cave[vecPosition + (n+1)].showSymbol() == 'W'){
						std::cout << "You shot the Wumpus!\n";
						cave[vecPosition + (n+1)].shotWumpus();
						return;
					}
				}
			}
		}

		int move = rand() % 10;
		if (move > 7){
			moveWumpus();
			std::cout << "The wumpus has heard your shot has moved locations.\n";
		}
	}
}

//move the wumpus' location
//this function is only utalized by the shoot funciton above
void Game::moveWumpus(){
	for(int n = 0; n < cave.size(); n++){
		if(cave[n].showSymbol() == 'W'){
			int x;
			srand(time(NULL));
			do{
				x = rand() % cave.size();
			}while(cave[x].showSymbol() != ' ');
			std::iter_swap(cave.begin() + n, cave.begin() + x);

		}
	}	

}

//this function was used only to develope the program and see where unique events are
void Game::printCave(){
	
	std::cout << " " ;
	for(int col = 0; col < this->size; col++){
		std::cout<< " | " << col + 1;
	}
	std::cout << " |\n";
	
	printFloor();

	for (int row = 0; row < this->size; row++){
		std::cout << row ;
		for(int col = 0; col < this->size; col++){
			std::cout<< " | " << cave[row*this->size + col].showSymbol();
		}
		std::cout << " |\n";
		printFloor();
	}
}

//outputs a display of the cave grid
void Game::showPosition(){
	
	//label for the grid
	std::cout << "\nCave: \n";					

	//save the vector indexed position of the players location
	int* position = this->player.currentPosition();			
	int vecLocation = (position[1]*this->size + position[0]);
	
	//print top of the grid
	printFloor();

	//prints each level of the grid start with the first element in the top left of the grid
	for (int row = 0; row < this->size; row++){
		for(int col = 0; col < this->size; col++){
			std::cout<< " | " ;
			if (vecLocation == (row*this->size + col)){
				std::cout << 'X' ;
			} else {
				std::cout << ' ';
			}
		}
		//prints final end of grid and the line between levels in the grid
		std::cout << " |\n";
		printFloor();
	}
}

//prints floor or top of grid
//for use only in grid display function only
void Game::printFloor(){
	std::cout << " " ;
	for(int n = 0; n < this->size*4 + 1; n++){
		std::cout<< "-";
	}
	std::cout << std::endl;
}

//generates and fills grid with all unique events
void Game::generateEvents(){
		
	int x;

	//store the rope location so that we dont place an event at this location
	int ropePosition = (this->exitRope[1]*this->size + this->exitRope[0]);

	//seed the rand function
	srand(time(NULL));

	//select a random element in the vector and check to make sure that it is empty and not at the rope location
	for (int n = 0; n < 6; n++){
		do{
			x = rand() %(this->size*this->size);

		}while(cave[x].showSymbol() != ' ' || x == ropePosition);
	
	//store the unique event based on number of current loop 
	if( n < 2){	
		cave[x].changePit();
	} else if (n < 4) {
		cave[x].changeBats();
	} else if (n==4) {
		cave[x].changeWumpus();
	} else {
		cave[x].changeGold();
	}

	}
}

//check to see if the wumpus is dead
bool Game::deadWumpus(){
	
	//scan all the values in the vector and see if they contain the wumpus
	//returns false if there is a wumpus
	for(int n = 0; n = (cave.size()); n++){
		if(cave[n].showSymbol() == 'W')
			return false;
	}
	//if no wumpus if found return true
	return true;
}

//check to see if the game is over
//the game is over when the user is dead or they have the gold, are at the exit rope and the wumpus is dead
bool Game::gameOver(){
	
	//if user is dead then return true
	if(this->player.isAlive() == false)
		return true;
	
	//check if winning conditions are met
	int* position = this->player.currentPosition();
	if (position[0] == exitRope[0] && position[1] == exitRope[0]){
		if(this->player.hasGold() == true && deadWumpus() == true){
			std::cout << "You have killed the wumpus and escaped with the gold!\n";
			return true;
		}
	}	
	
	return false;
}

//function checks the adjacent rooms for unique events and outputs the percepts if there are any
void Game::checkRooms(){
	//store the users current vector indexed address
	int* position = this->player.currentPosition();
	int vecPosition = (position[1]*this->size + position[0]);
	//set the maximum index an element in the vector can access
	int max = this->size*this->size;

	//scan the adjacent rooms 
	//if the room isn't empty display the rooms percept
	if (cave[vecPosition - 1].showSymbol() != ' ' && (vecPosition - 1) > 0)
		cave[vecPosition - 1].displayPercept();

	if (cave[vecPosition + 1].showSymbol() != ' ' && (vecPosition + 1) < max)
		cave[vecPosition + 1].displayPercept();

	if (cave[vecPosition - this->size].showSymbol() != ' ' && (vecPosition - this->size) > 0)
		cave[vecPosition - this->size].displayPercept();

	if (cave[vecPosition + this->size].showSymbol() != ' ' && (vecPosition + this->size) < max)
		cave[vecPosition + this->size].displayPercept();
}

//this function will move a player one room 
//utalized every turn
void Game::movePlayer(){

	//store the vector index of the players current position
	int* position = this->player.currentPosition();
	//store variables for current grid x and y coordinates as well as moved distance variables
	int positionX, positionY, dx, dy;
	positionX = position[0];
	positionY = position[1];

	//variable to store the users' input
	std::string input;
	do{
		//make sure to reset the moved distance variables every time we reprompt
		dx = 0;
		dy = 0;
		//prompt for a direction for a direction to move
		do{
			std::cout << "Which way would you like to proceed?\n";
			std::cout << "Enter up, down, left, or right: ";
			std::cin >> input;
		}while(input != "up" && input != "down" && input != "left" && input != "right");
	
		//trenslate the users specified move to a numeric change that will be applied to the position
		if(input == "up")
			dy = -1;
	
		if(input == "down")
			dy = 1;
	
		if(input == "left")
			dx = -1;

		if(input == "right")
			dx = 1;
	//reprompt every when the users picks a move that places them out of the range of our grid
	}while(positionX + dx < 0 || positionX + dx >= this->size || positionY + dy < 0 || positionY + dy >= this->size);

	//finally call a moving function for the player
	this->player.move(dx,dy);

}

//this function will activate the action at the users current location
void Game::roomAction(){
	//for(int n = 0; n < 2; n++){
	int vecPosition;
	//continue looping the action function while we experince the bat event so we react to the event we are dropped into
	do{
		int* position = this->player.currentPosition();
		vecPosition = (position[1]*this->size + position[0]);
		cave[vecPosition].eventAction(this->player,this->size);
	}while(cave[vecPosition].showSymbol() == 'B');
			
}
