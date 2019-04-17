
#ifndef __CARD_HPP
#define __CARD_HPP

class Card{

	private:
		int rank;	//range: 0-12
		int suit;	//range: 0-3

	public:
		Card();				//default constructor
		Card(int,int);			//parameterized constructor
		void show_card();		//show card suit and rank 
		int get_suit();			//return suit
		int get_rank();			//return rank
		void change_card(int,int);	//change the value the card
		//must have constructors, destructor, accessor methods, and mutator methods
		//
};


#endif
