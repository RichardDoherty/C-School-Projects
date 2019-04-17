#include "Hand.hpp"
#include <iostream>
#include <iomanip>

/*Default constructor that will initialize n_cards to 0
 */
Hand::Hand() : n_cards(0) { }

/*This function will take the top card from the dect and add
 *it to a hand
 */
void Hand::draw_card(Deck& deck) {
	this->cards.push_back(deck.draw());
	this->n_cards++;
}

/*This function will deal the hand to the players
 */
void Hand::get_hand(Deck& deck) {
	for (int n = 0; n < 7; n++) {
		this->cards.push_back(deck.draw());
		this->n_cards++;
	}
}

/*This function will show all the cards in the users hands
 */
void Hand::show_hand() {

	if (this->n_cards > 0){	
		for (int n = 0; n < this->n_cards; n++) {
			std::cout << "Card " << n+1 << ":" ;
			if (n+1 > 9){
				std::cout << std::setw(9);
			} else {
				std::cout << std::setw(10);
			}
			this->cards[n].show_card();
		}
	}
}

/*This function will check to see in the card entered in the parameter
 *is in the players hand
 */
bool Hand::check_cards(int card) {

	if (this->n_cards > 0) {
		for (int n = 0; n < this->n_cards; n++) {
			if (this->cards[n].get_rank() == card) {
				return true;
			}
		}
	}	
	return false;
}

/*This function will remove and return a card from on players hand
 */
Card Hand::give_card(int card) {

	for (int n = 0; n < this->n_cards; n++) {
		if (this->cards[n].get_rank() == card) {
			Card give_card = cards[n];
			cards.erase (cards.begin()+n);
			this->n_cards--;
			return give_card;
		}
	}
}

/*This function will recieve a card in the parameter and 
 *then store the card in the user's hand
 */
void Hand::get_card(Card card) {

	this->cards.push_back(card);
	this->n_cards++;
}

/*Function returns the number of cards in a players hand
 */
int Hand::num_cards() { 
	
	return this-> n_cards;
}

/*This function will get the rank from a specific card
 *in a users hand
 */
int Hand::get_rank(int n) {

	return this->cards[n].get_rank();
}

/*This funtion will scan for and remove books from a users hand
 */
void Hand::remove_book(int book) {
	int* book_card = new int[4];
	int count = 0;

	for (int n = 0; n < n_cards; n++) {
		if (cards[n].get_rank() == book){
			book_card[count] = n;
			count++;
		}	
	}

	int offset = 0;
	for (int n = 0; n < 4; n++) {
		cards.erase (cards.begin()+(book_card[n]-offset));
		n_cards--;
		offset++;
	}
}

