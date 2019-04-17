#include "player.hpp"
#include "room.hpp"
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include "time.h"
#ifndef __GAME_HPP
#define __GAME_HPP

	class Game {
	
		private: 
			Player player;	
			int size;			//size of grid
			std::vector<Room> cave;		//used one dimensional vector for grid. Requires manual indexing
			int exitRope[2];		//saves the location of the escape rope
		public: 
			Game(int);			//parameterized constructor
			void printCave();
			void printFloor();		//prints line for grid
			void movePlayer();		//moves a player each turn
			void generateEvents();		
			void shootArrow();		//allows user to attempt to shoot wumpus
			void showPosition();		//display grid
			bool gameOver();		//checks if end game conditions are met
			bool deadWumpus();		//check is the wumpus is dead
			void checkRooms();		//checks surrounding rooms for percepts
			void roomAction();		//conducts the users current rooms' event action
			void moveWumpus();		//moves wumpus location
	};
#endif
