#include "Game.hpp"
#include <iostream>
#include <time.h>
#include <stdlib.h>

/*This function is the default constructor called for game 
 *this function will welcome the user and shuffle the cards 
 *in the deck
 * */
Game::Game(){

	std::cout << "\nWelcome to a game of go fish!\nYou will be Player 1 and Player 2 will be a computer opponent.\n" << std::endl;	
	this->cards.shuffle();
}

/*This function will deal the cards to each user by calling the 
 *get_hand function for the hand class
 */
void Game::deal() {

	for (int n = 0; n < 2; n++) {
		this->players[n].get_hand(this->cards);
	}

}

/*This function will be used to show the user the cards in their hand
 *at the start of their turn by calling the show hand function for a player
 */
void Game::show_hand(int player) {

	std::cout << "\nYour hand: " << std::endl;
	this->players[player].show_hand();
	std::cout << std::endl;				//extra space between prompt and cards
	players[player].show_books();
}

/*This function returns the proper array index of the users entered
 *input for a card selection
 */
int convert_input(std::string input) {
	
	if ( input == "2") return 0;
	
	if ( input == "3") return 1;

	if ( input == "4") return 2;
	
	if ( input == "5") return 3;
	
	if ( input == "6") return 4;
	
	if ( input == "7") return 5;
	
	if ( input == "8") return 6;
	
	if ( input == "9") return 7;
	
	if ( input == "10") return 8;
	
	if ( input == "J") return 9;
	
	if ( input == "Q") return 10;
	
	if ( input == "K") return 11;
	
	if ( input == "A") return 12;
	
	return 13;
}

/*This function will take the player through the process of 
 *asking for a card and either drawing from the deck or taking 
 *a card from the players opponent
 */
void Game::player_turn(int player) {
	
	int opp, card;
	std::string card_val;
	//define if the opponent player for the turn
	if (player == 0) {
		opp = 1;
	} else {
		opp = 0;
	}	

	//get a rank to ask for from the user
	//if the computer is the one asking for a card then randomly select a card value from 
	//the computer's hand
	if (player == 0) {
		do {
			std::cout << "Select a rank to request: " ;
			std::cin >> card_val;
			card = convert_input(card_val);
			
			if(card != 13 && players[player].n_cards() == 0) {
				break;
			}

		} while(card == 13 || players[player].check_hand(card) == false);

	} else {
		srand(time(NULL));
		if (players[player].n_cards() > 0){
			int n = rand() % players[player].n_cards();
			card = players[player].card_rank(n);
		} else {
			card = rand() % 13;
		}

	}
	
	//if the player asks for a cad that the other player has then they must give them the card
	if(this->players[opp].check_hand(card) == true) {
		
		std::cout << "\nPlayer " << player + 1 << " guessed correct!\n";
		do {
			players[player].get_card(players[opp].give_card(card));
		} while(this->players[opp].check_hand(card) == true);

	} else { //the player asked for a card their opponent did not have
		std::cout << "\nPlayer " << player+1 << " go fish!" << std::endl;
		if (this->cards.n_deck() != 0) {
			players[player].draw(cards);
		}
	}
}

/*This function will remove any four of a kind cards from a players hand
 *and then it will increment the number of books and fill in a one in the appropriate 
 *books array
 */
void Game::move_books(int player) {
	players[player].move_books();
}

/*This function will total and return the number of books from both players
 */
int Game::total_books(){
	return (this->players[0].num_books() + this->players[1].num_books());
}

/*This function decides and announces the winner of the game
 */
void Game::winner() {
	if (players[0].num_books() > players[1].num_books()) {
		std::cout << "Congratulations! You have won the game!!" << std::endl;
	} else {
		std::cout << "You lose! Better luck next time!" << std::endl;
	}
}

/*This function shows the users books
 */
void Game::show_books(int player) { 
	players[player].show_books();
}
