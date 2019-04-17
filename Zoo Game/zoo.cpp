#include <iostream>
#include "zoo.hpp"
#include <stdlib.h>
#include <time.h>

//Default parameter for zoo welcomes user, initiates money to $100,000 and number animals to zero
Zoo::Zoo() : balance(100000) , n_animals(0) {

	std::cout << "Welcome to Zoo Tycoon!\n";
}

//function gets a random animal in zoo sick
//Preconditions: the zoo contains an adult animal
//Postconditions: 
void Zoo::sick(){
	
	srand(time(NULL));
	int rand_species, rand_animal;

	if (this->rand_event == 1 && n_animals > 0){		//if the random event is set 1 it means animal birth
		do {
			rand_species = rand() % 3 + 1;		//value from 1-4
		
		} while (!(rand_species == 1 && this->monkeys.size() > 0) && !(rand_species ==2 && this->sloths.size() > 0)
			       	&& !(rand_species == 3 && this->otters.size() > 0));	//loop selects the first species with more than one animal
		
		//check if each species of animal is randomly selected and makes a random animal sick if selected
		if (rand_species == 1){ 
			
			rand_animal = rand() % this->monkeys.size();
			this->monkeys[rand_animal].sick();
		
		}else if (rand_species == 2){

			rand_animal = rand() % this->sloths.size();
			this->sloths[rand_animal].sick();
		
		} else {
			rand_animal = rand() % this->otters.size();
			this->otters[rand_animal].sick();
		}
			
	}

}

//check if any animals in the zoo are sick
void Zoo::check_sick() {

	std::string save;

	//the following three loops are used to check each species
	//use for loops to cycle through every animal in monkey, sloth, and otter array
	//if the animal is sick tell the user and print the sick animals values 
	//then prompt the user to pay the medical costs
	//if the user does not pay the medical cost the animal dies 
	//if the user saves the animal the medical cost amount is subtracted out of balance and the animals health is changed back to false
	
	for (int n = 0; n < (this->monkeys.size()); n++){		
		if (monkeys[n].is_sick() == true){
			std::cout << "Your monkey is sick!\nSick ";
		       	monkeys[n].show_values();
			std::cout << "Medical Cost: " <<monkeys[n].med_cost() << "\tMoney: $" << this->balance << "\nEnter yes to save animal: ";
			std::cin >> save;
			if (save == "yes" && this->balance > monkeys[n].med_cost()){
					this->balance-= this->monkeys[n].med_cost();
					this->monkeys[n].healthy();
			} else {
				if (this->balance < monkeys[n].med_cost()){
					std::cout << "You do not have enough to heal your animal!\n";
				}	
				this->monkeys.erase(monkeys.begin()+n);
			}
		}	       
	}


	for (int n = 0; n < (this->sloths.size()); n++){
		if (sloths[n].is_sick() == true){
			std::cout << "Your sloth is sick!\nSick ";
		       	sloths[n].show_values();
			std::cout << "Medical Cost: " <<sloths[n].med_cost() << "\tMoney: $" << this->balance << "\nEnter yes to save animal: ";
			std::cin >> save;
			if (save == "yes" && this->balance > sloths[n].med_cost()){
					this->balance-= this->sloths[n].med_cost();
					this->sloths[n].healthy();
			} else {
				if (this->balance < sloths[n].med_cost()){
					std::cout << "You do not have enough to heal your animal!\n";
				}	
				this->sloths.erase(sloths.begin()+n);
			}
		}	       
	}

	for (int n = 0; n < (this->otters.size()); n++){
		if (otters[n].is_sick() == true){
			std::cout << "Your otter is sick!\nSick ";
		       	otters[n].show_values();
			std::cout << "Medical Cost: " <<otters[n].med_cost() << "\tMoney: $" << this->balance << "\nEnter yes to save animal: ";
			std::cin >> save;
			if (save == "yes" && this->balance > otters[n].med_cost()){
					this->balance-= this->otters[n].med_cost();
					this->otters[n].healthy();
			} else {
				if (this->balance < otters[n].med_cost()){
					std::cout << "You do not have enough to heal your animal!\n";
				}	
				this->otters.erase(otters.begin()+n);
			}
		}	       
	}
}

//pick a random event for the day
void Zoo::randomize_event() {
	srand(time(NULL));						//seed the random variable
	this->rand_event = rand() % 4 +1;				//values from 1-4
	
	std::cout<< "Random Event: " << this->rand_event << std::endl;	//print the random event to the user to prove its random and accurate
}

//choose to purchase any animals
//Postconditions: the zoo will gain either nothing, one adult animal, or two adult animals
void Zoo::buy_animal(){
	bool afford;				//variable will check if a user can afford an animal
	std::string selection, num;
	int n;

	do {
		afford = true;			//initialize afford to true each time the user picks an animal
		do {
			std::cout << "Would you like to buy a monkey($15,000), sloth ($2,000), or otter($5,000)? Enter none to buy no animals.\nSelection: " ;
			std::cin >> selection;
			
			if (selection == "none" )		//the user has selected to not buy any animals
				return;

			if (selection!= "monkey" && selection != "sloth" && selection != "otter"){	//error message for bad input
				std::cerr << "Error! Enter ""sloth"", ""otter"" or ""monkey""!\n";
			}

		} while(selection!= "monkey" && selection != "sloth" && selection != "otter");		//check for back input
	
		
		do {											//determine the num animals a user wants
			std::cout << "Would you like 1 or 2 "<<selection<< "s?\nSelection: ";
			std::cin >>num;
		} while (num != "1" && num != "2");							//check user entered 1 or 2
		
		if (num == "1"){			//convert string values to integers
			n = 1;
		} else {
			n = 2;
		}

		//the following code check if a user can afford the animals they are attempting to buy
	
		if(selection == "monkey"){
			if(this->balance < (15000*n)){
				std::cerr << "You cannot afford your " << selection << " selection\n";
				afford = false;	
				}
		} else if (selection == "sloth") {
			if(this->balance < (2000*n)){
				std::cerr << "You cannot afford your " << selection << " selection\n";
				afford = false;	
			}
		} else {
			if(this->balance < (5000*n)){
				std::cerr << "You cannot afford your " << selection << " selection\n";
				afford = false;	
			}
		}
	} while(afford == false);		//continue to loop until the user makes a seletion they can afford
	
	this->n_animals+= n ;			//increment n_animals by the number of animals bought
	
	//add purchased animals to dynamic arrays in zoo

	if(selection == "monkey"){
		for (int a = 0; a < n ; a++){
			this->monkeys.push_back(Monkey(1095));
			this->balance-= this->monkeys[0].animal_cost();
		}
	}

	if(selection == "sloth"){
		for (int a = 0; a < n ; a++){
			this->sloths.push_back(Sloth(1095));
			this->balance-= this->sloths[0].animal_cost();
		}
	}

	if(selection == "otter"){
		for (int a = 0; a < n ; a++){
			this->otters.push_back(Otter(1095));
			this->balance-=  this->otters[0].animal_cost();
		}
	}
}

//show the names, ages, and age statuses of all animals in the zoo as well as the zoo's balance
void Zoo::show_values() {
	
	std::cout << "Money: $" << this->balance << "\nCurrent animals: \n" ;		//output the zoos current balance

	//output all animals with their names, ages, and age statuses
	for (int n = 0; n < this->monkeys.size(); n++){
		monkeys[n].show_values();
	}

	for (int n = 0; n < this->otters.size(); n++){
		otters[n].show_values();
	}

	for (int n = 0; n < this->sloths.size(); n++){
		sloths[n].show_values();
	}
}

//function will subtract food cost from your balance everyday
void Zoo::feed_animals(){

	if (this->n_animals > 0) {	
		float food;
	
		srand(time(NULL));						//calculate the random base cost of all food
		food = rand()% 26 + 37.50;
	
		std::cout << "Base food cost: " << food << std::endl;		//report the base cost of food

		//for each animal multiply the base cost by their unique food cost ratio
		for (int n = 0; n < this->monkeys.size(); n++){
			this->balance-= (monkeys[n].cost_food()*food);
		}
	
		for (int n = 0; n < this->otters.size(); n++){
			this->balance-= (otters[n].cost_food()*food);
		}
	
		for (int n = 0; n < this->sloths.size(); n++){
			this->balance-= (sloths[n].cost_food()*food);
		}
	}
}

//check for bankrupcy in the zoo
bool Zoo::bankrupt() {
	if (balance < 0){
		return true;
	} else {
		return false;
	} 
}

//advances the age of all animals by one day
void Zoo::advance_day() {
	
	for (int n = 0; n < this->monkeys.size(); n++){
		monkeys[n].advance_day();
	}

	for (int n = 0; n < this->otters.size(); n++){
		otters[n].advance_day();
	}

	for (int n = 0; n < this->sloths.size(); n++){
		sloths[n].advance_day();
	}
}

//adds up all revenue and print the total revenue
void Zoo::calculate_revenue() {

	int total = 0;
	int bonus;
	if (this->rand_event ==2)						//print the boom in attendence  if it is the  random event
		std::cout<< "Random boom in zoo attendence for the day!\n";
	
	for (int n = 0; n < this->monkeys.size(); n++){				//revenue for baby monkey w/o boom
		if (monkeys[n].animal_age() < 30){
			this->balance += 2*monkeys[n].animal_revenue();
			total += 2*monkeys[n].animal_revenue();
			if(this->rand_event ==2){
				bonus =  rand() % 151 + 350;			//revenue for baby monkey w/ boom
				this->balance += bonus;
				total += bonus;
			}
		} else {
			this->balance += monkeys[n].animal_revenue();		//revenue for non baby monkeys
			total += monkeys[n].animal_revenue();
			if(this->rand_event ==2){
				bonus = rand() % 151 + 350;			//revenue for non-baby monkeys w/ boom
				this->balance += bonus;
				total += bonus;
			}
		}
	}

	for (int n = 0; n < this->otters.size(); n++){
		if (otters[n].animal_age() < 30){				//revenue for baby otters 
			this->balance += 2*otters[n].animal_revenue();
			total += 2*otters[n].animal_revenue();
		} else {
			this->balance += otters[n].animal_revenue();		//revenue for non-baby  otters
			total += otters[n].animal_revenue();
		}
	}

	for (int n = 0; n < this->sloths.size(); n++){				//revenue for  baby sloths
		if (sloths[n].animal_age() < 30){
			this->balance += 2*sloths[n].animal_revenue();
			total += 2*sloths[n].animal_revenue();
		} else {
			this->balance += sloths[n].animal_revenue();		//revenue for non-baby sloths
			total += sloths[n].animal_revenue();
		}
	}
	std::cout << "Daily revenue: $" << total << std::endl;
}

//checks if there are any adults in the zoo and returns a boolean value
bool Zoo::adults(){
	
	//checks every animal of every species
	for (int n = 0; n < this->monkeys.size(); n++){
		if (monkeys[n].animal_age() >= 1095)
			return true;
	}

	for (int n = 0; n < this->otters.size(); n++){
		if (otters[n].animal_age() >= 1095)
			return true;
	}

	for (int n = 0; n < this->sloths.size(); n++){
		if (sloths[n].animal_age() >= 1095)
			return true;
	}
	
	//returns false if none of the animals are adult
	return false;
}

//births a random adult animals with there are any
//Precondition: there is an adult animal at the zoo 
void Zoo::birth(){


	if (this->rand_event == 3){							//randomly select an animal 
		std::cout << "One of your animals is giving birth!\nBirth ";		//inform the user of birth
		srand(time(NULL));
		do{
			int species = rand() % 3 + 1;
			if (species == 1 && this->monkeys.size() > 0){		//if monkey is selectedn and theres an  adult monkey birth a baby
	
				for (int n = 0; n < this->monkeys.size(); n++){ 
					if (monkeys[n].animal_age() >= 1095){
						monkeys[n].show_values();		//show what monkey is giving birth
						this->monkeys.push_back(Monkey(0));
						this->n_animals++;
						return;
					}
					
				}
			}


			if (species == 1 && this->sloths.size() > 0){
	
				for (int n = 0; n < this->sloths.size(); n++){	//if sloth is selected and theres an adult sloth birth a baby
					if (sloths[n].animal_age() >= 1095){
						sloths[n].show_values();	//show the parent sloth
						this->sloths.push_back(Sloth(0));
						this->n_animals++;
						return;
					}
					
				}
			}

			if (species == 3 && this->otters.size() > 0){
	
				for (int n = 0; n < this->otters.size(); n++){
					if (otters[n].animal_age() >= 1095){	//if otter is selected and theres and adult otter birth a baby
						otters[n].show_values();	
						this->otters.push_back(Otter(0));	//show the parent otter
						this->n_animals++;
						return;
					}
					
				}
			}
		}while(this->rand_event != 4);

	}
}

//deallocate dynamic memory created by animal vectors
Zoo::~Zoo(){
	this->monkeys.erase(this->monkeys.begin(),this->monkeys.begin()+(this->monkeys.size()));
	this->sloths.erase(this->sloths.begin(),this->sloths.begin()+(this->sloths.size()));
	this->otters.erase(this->otters.begin(),this->otters.begin()+(this->otters.size()));
}
