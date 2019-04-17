#include <iostream>
#include "Card.hpp"
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <stdio.h>
/*Default constructor that sets the rank and suit 
 *to zero
 */
Card::Card() : rank(0), suit(0) {}


/*Parameterized constructor that takes in two ints
 *and stores the first as the cards suit and the 
 *second is the cards rank
 */
Card::Card(int suit,int rank) {
	this->rank = rank;
	this->suit = suit;
}

/*This function will display the users card rank
 *and suit 
 */
void Card::show_card() {
	char s;
	if (this->suit == 0) 
		s = 'S';	
	if (this->suit == 1) 
		s = 'H';	
	if (this->suit == 2) 
		s = 'C';	
	if (this->suit == 3) 
		s = 'D';	


	std::cout  << "Suit: " << s << std::setw(10) << "Rank: ";

	if (this->rank >= 0 && this->rank <= 8) {
		std::cout <<  (this->rank + 2) << std::endl;
	} else {
	if (this->rank == 9){ 
		std::cout << "J" << std::endl;	
	}
	if (this->rank == 10) {
		std::cout << "Q" << std::endl;	
	}
	if (this->rank == 11) {
		std::cout << "K" << std::endl;	
	}
	if (this->rank == 12) {
		std::cout << "A" << std::endl;
	}
	}
}

/*This function will change the rank and suit values associated with a card
 */
void Card::change_card(int suit, int rank) {
	this->rank = rank;
	this->suit = suit;
}

/*This function will return the integer suit value
 */
int Card::get_suit() {
	return this->suit;
}

/*This function will return the integer rank value
 */
int Card::get_rank() {
	return this->rank;
}
