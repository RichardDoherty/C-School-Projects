#include "Card.hpp"
#include "Deck.hpp"
#include <vector>

#ifndef __HAND_HPP
#define __HAND_HPP

class Hand{

	private:
		std::vector<Card> cards;	//array of cards in hand
		int n_cards;	//number of cards in hand
	public:
		//must have constructors, destructor, accessor, methods, and mutator methods
	Hand();
	void get_hand(Deck&);
	void show_hand();
	bool check_cards(int);
	Card give_card(int);
	void get_card(Card);
	void draw_card(Deck&);
	int num_cards();
	int get_rank(int);	
	void remove_book(int);
};

#endif
