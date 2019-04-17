#include "Card.hpp"

#ifndef __DECK_HPP
#define __DECK_HPP

class Deck {
	private:
		Card cards[52];
		int n_cards;
	public:
		Deck(); 	//must have constructors, desructor, accessor methods and mutator methods
		//~Deck();
		void shuffle();		//shuffles the deck randomly
		void show_card(int);	//shows card in deck specified by user REMOVE BEFORE SUBMISSION
		int n_deck();		//returns number of cards in deck
		Card draw();	
	};

#endif
