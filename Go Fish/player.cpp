#include "Player.hpp"
#include <iostream>
#include <algorithm>

/**Default constructor for Player class**/
Player::Player() : n_books(0){
	this->books = new int[13] ;
}

/*Default deconstructor
 */
Player::~Player() {
	delete[] this->books;
}

/*This function will call the get_hand() from the hand class
 */
void Player::get_hand(Deck& deck) {
	this->hand.get_hand(deck);

}

/*This will show the cards in the users hand by calling the show_hand()
 * funtion from the hand class
 */
void Player::show_hand() {

	this->hand.show_hand();
}

/*This will check if a card is in a players hand by calling
 *the check cards funtion in a players hand
 */
bool Player::check_hand(int card) {
	
	return this->hand.check_cards(card);

}

/*This function calls the draw_card funtion from the hand class
 */
void Player::draw(Deck& deck) {
	
	hand.draw_card(deck);

}

/*This function will return a card that will be given by calling the 
 * give card function from the hand class
 */
Card Player::give_card(int card) {
	return this->hand.give_card(card);
}

/*This function calls the get_card function in the hand class
 */
void Player::get_card(Card card) {
	this->hand.get_card(card);
}

/*This will return the number of cards in a players hand by calling
 * the num_cards function in the hand class
 */
int Player::n_cards() {
	this->hand.num_cards();
}

/*This will get the rank of a specified card in a players hand by 
 * calling the get_rank function of the hand class
 */
int Player::card_rank(int n) {
	this->hand.get_rank(n);
}

/*This will return the number of books a player has
 */
int Player::num_books() {
	return this->n_books;
}

/*Compare function for an array of rank values
 */
bool cmp_ranks(const int& rank1, const int& rank2) {
	return (rank1 < rank2);
}

/*This will call the remove books funtion from the hand class and will 
 * this funtion will store these values in an array and then use the sort
 * function to arrange them in order so that a four of a kind can be identified
 */
void Player::move_books() {

	if (this->hand.num_cards() >= 4) {
		int *temp_hand = new int[this->hand.num_cards()];
	
		for (int n = 0; n < this->hand.num_cards(); n++){
			temp_hand[n] = this->hand.get_rank(n);
		}
		std::sort(temp_hand,temp_hand+(this->hand.num_cards()),cmp_ranks);

		for (int n = 0; n < this->hand.num_cards(); n++){
			if (temp_hand[n] == temp_hand[n+3]){
				int book = temp_hand[n];		//store the rank of the book
				this->hand.remove_book(book);
				this->n_books++;
				this->books[book] = 1;
				break;
			}
	
		}
	delete [] temp_hand;
	}
}

void Player::show_books() {
	std::cout << "Your books: \n";
	std::cout << "2: " << books[0] << "\t3: " << books[1] << "\t4: " << books[2] << "\t5: " << books [3] ;
	std::cout << "\t6: " << books[4] << "\t7: " << books[5] << "\t8: " << books[6] << "\t9: " << books[7];
        std::cout << "\t10: " << books[8] << "\tJ: " << books[9] << "\tQ: " << books[10] << "\tK: " << books[11];
        std::cout << "\tA: " << books[12] << std::endl;

}
