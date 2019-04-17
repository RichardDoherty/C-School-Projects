#include "Deck.hpp"
#include <iostream> 
#include <stdlib.h>	//rand library
#include <time.h> 	//seed rand

/*This is the default constructor that will initialize the number of cards in the deck
 *and will fill the cards with ranks and values
 */
Deck::Deck() {
	
	this->n_cards = 52;
	//std::cout << "Default card constructor called" << std::endl;	
	int card = 0;

	for (int suit = 0; suit < 4; suit++) {
		for (int rank = 0; rank < 13; rank++){
			this->cards[card] = Card(suit,rank);
			card++;
		}
	}

}

/*This function will shuffle all the cards in the deck by storing
 *them randomly and temporarily in an array before restoring the values
 *in the deck
 */
void Deck::shuffle() {

	srand(time(NULL));

	int** temp_deck;
	temp_deck = new int*[52];
	int card;

	for (int n = 0; n < 52; n++) {
		temp_deck[n] = new int[2];
	}
	
	for (int n = 0; n < 52; n++) {
		do{	
	//		std::cout << "Generate random for: ";
			card = rand() % 52;
	//		std::cout << card << "\nCard value: " << cards[card].get_suit() << std::endl;
		}while(cards[card].get_suit() == 4);

		temp_deck[n][1] = cards[card].get_suit();
		temp_deck[n][2] = cards[card].get_rank();
	
		cards[card].change_card(4,52);
		
	}
	
	for (int n = 0; n < 52; n++) {
		
		cards[n].change_card(temp_deck[n][1],temp_deck[n][2]);
	
	}
	
	for (int n = 0; n < 52; n++) {
		delete [] temp_deck[n];
	}
	delete [] temp_deck;
}

/*This function returns the number of cards in a deck
 */
int Deck::n_deck() {
	return this->n_cards;
}

/*This function will return the top card from the deck and 
 *decrement the n_cards value
 */
Card Deck::draw() {
	
	this->n_cards--;

	return this->cards[n_cards];	
}
