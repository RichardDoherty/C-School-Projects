#include <iostream>
#include <vector>
#include "zoo.hpp"
int main (int agrc, char** argv) {
	
	int day = 0;						//create day tracker
	Zoo zoo;						//create zoo
	do {
		std::cout << "Day: " << day << std::endl;	//display the day
		zoo.randomize_event();				//pick a daily random event
		if (day > 0){
			zoo.feed_animals();			//They to feed animals if past initial day
		}		
		zoo.sick();					//get animals sick if sick rand event is choosen
		zoo.check_sick();				//check for sick animals
	
		if (zoo.adults() == true)			//if there are adults in the zoo
			zoo.birth();				//birth a baby if birth rand eveny is choosen

		zoo.show_values();				//show the zoos money and animals
		zoo.buy_animal();				//offer the user to buy animals
		zoo.calculate_revenue();			//calculate revenue for the day
		zoo.advance_day();				//age animals
		day++;						//incremanet day counter
		std::cout << std::endl;				
	} while (zoo.bankrupt() == false);			//check if the zoo is out of money
	
	std::cout <<"You went bankrupt!" << std::endl;		//inform the user if bankrupt
	return 0;
}
