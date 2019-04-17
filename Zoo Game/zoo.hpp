#include <vector>
#include "monkey.hpp"
#include "otter.hpp"
#include "sloth.hpp"
#ifndef __ZOO_HPP
#define __ZOO_HPP
class Zoo {

	private:
		float balance;				//money
		int n_animals;				//number of animals in zoo
		std::vector<Monkey> monkeys;		//dynamic array of monkeys
		std::vector<Otter> otters;		//dynamic array of otters
		std::vector<Sloth> sloths;		//dynamic array of sloths
		int rand_event;				//tracks current random event of the day
	public:
		Zoo();					//Default constructor
		~Zoo();					//Default deconstructor
		void buy_animal();			//purchase and add an animal to zoo
		void show_values();			//show the zoo's balance and animals
		void feed_animals();			//feed animals and pay food costs
		bool bankrupt();			//boolean check to see if zoo is bankrupt
		bool check_balance();			//checks if a user can purchase an animal
		void advance_day();			//advance zoo and animals by one day
		void birth();				//births a random adult animal
		void calculate_revenue();		//totals the individual revenues from all animals and events and adds the value to balance
		void sick();				//makes an adult animal sick
		void check_sick();			//checks if any animals in the zoo are sick
		void randomize_event();			//chooses random event for the day
		bool adults();				//checks to see if there are adult animals at the zoo

};
#endif
