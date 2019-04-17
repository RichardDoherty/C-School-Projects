#include "Deck.hpp"
#include "Player.hpp"
#ifndef __GAME_HPP
#define __GAME_HPP

class Game{

	private: 
		Deck cards;		//cards in deck
		Player players[2];	//players in the game

	public:
		Game();			//default constructor 
		bool game_over();
		void deal();
		void show_hand(int);
		int n_cards();
		void player_turn(int);
		void move_books(int);
		int total_books();
		void winner();
		void show_books(int);
};

#endif
