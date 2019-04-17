#include <iostream>
#include <stdlib.h>
#include "game.hpp"
int main (int argc, char** argv){

	if (argc > 1){	
	int size = atoi(argv[1]);			//store the command line parameter as an integer

	if (size >= 4){
		int x;
		Game game(size);			//create a grid of the specified size
		game.generateEvents();			//fill the rooms in grid with events
		do{	
			game.showPosition();		//show the grid of rooms and the users' position
			game.checkRooms();		//check the adjacent rooms for events
			game.shootArrow();		//ask the user if they would like to try to shoot an arrow at the Wumpus
			game.movePlayer();		//move the users position
			system("clear");
			game.roomAction();		//conduct the action that the user has landed on
		}while(game.gameOver() == false);	//test if the user has gold and wumpus is dead or if player is dead

	} else {
		
		std::cout << "Enter a command line argument that is greater than 4.\n"; //error handle if number entered is less than 4
	}
	} else {
		std::cout << "Enter the length of your desired square playing grid in the command line\n" ; //error handle if no input entered

	}	
	
	return 0; 
}



