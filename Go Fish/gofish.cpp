#include <iostream>
#include <string.h>
#include "Card.hpp"
#include "Deck.hpp"
#include "Hand.hpp"
#include "Player.hpp"
#include "Game.hpp"

int main(int argc, char **argv){
	
	Game gofish;		//cards are automatically shuffled when default constructor is called 
	gofish.deal();		//deal cards to players
     
		
	gofish.move_books(0);	//move any books that the user may have randomly drawn
	gofish.move_books(1);
	
	int player = 0; 	//player 1 (the user) will start the game make player variable to track whos turn it is

	do {
		if (player%2 == 0){			//player%2 = 0 when the user is playing
			gofish.show_hand(player%2); 	//show the user their hand when they player
			//gofish.show_hand(1);
		}
		gofish.player_turn(player%2);		//execute the current players turn
		gofish.move_books(player%2);		//check for books for the current player
		player++;				//increment the player variable

	}while(gofish.total_books() != 13);		//if there are 13 total books then the game is done

	gofish.winner();

	return 0;		
}
