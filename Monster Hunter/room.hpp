#include "event.hpp"
#include "wumpus.hpp"
#include "bats.hpp"
#include "pit.hpp"
#include "player.hpp"
#include "gold.hpp"
#ifndef __ROOM_HPP
#define __ROOM_HPP

class	Room {

	private: 
		Event* event;
		int position[2];
		char symbol;
	public: 
		Room(int,int);
		~Room();
		char showSymbol();
		void changeWumpus();
		void changePit();
		void changeBats();
		void changeGold();
		void displayPercept();
		void eventAction(Player&,int);
		void shotWumpus();
};
#endif
