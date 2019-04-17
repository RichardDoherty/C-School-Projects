#include "stdlib.h"
#include "time.h"
#ifndef __PLAYER_HPP
#define __PLAYER_HPP

	class Player{

		private:
			int position[2];		//int array of the x and y value of the users location
			bool gold;			
			int nArrows;			//number arrows remaing
			bool alive;
		public:
			Player();
			void enter(int,int);
			int numArrows();
			bool isAlive();
			bool hasGold();	
			int* currentPosition();
			void move(int, int);
			void dead();
			void foundGold();
			void loseArrow();
			void transpose(int,int);
	};
#endif
