#include "Hand.hpp"
#ifndef __PLAYER_HPP
#define __PLAYER_HPP

class Player{

	private:
		Hand hand; 	//players hand
		int* books; 	
		int n_books;
	public:
		Player();// must have constructors, destructorm accessor methodsm and mutator methods
		~Player();
		void get_hand(Deck&); 
		void show_hand();
		bool check_hand(int);
		Card give_card(int);
		void get_card(Card);
		int n_cards();
		void draw(Deck&);
		int card_rank(int);
		int num_books();
		void move_books();	
		void show_books();
};

#endif
