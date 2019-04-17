#include <iostream>
#ifndef __ANIMAL_HPP
#define __ANIMAL_HPP
class Animal {
	
	public: 
		std::string name;
		std::string age_status;
		int age;
		int cost;
		int n_babies;
		int food_cost;
		int revenue;
		bool health;			//will be used to indicate sick animals

	public:
		Animal(std::string,int,int,int,int,int);	//default constructor called by each child class during its construction
		int animal_age();				//returns age
		void show_values();				//shows name and age of an animal
		void advance_day();				//advances an animals age by one day
		int animal_cost();				//returns the purchasing cost of one animal
		void age_check();				//checks an animals age and adjust it's age status accordingly
		int cost_food();				//returns the cost to feed an animal given its age
		void sick();					//will be used to make an adult animal sick
		bool is_sick();					//boolean returns if animal is sick or not
		int med_cost();					//returns the cost to cure a sick animal
		int animal_revenue();				//returns the amount of revenue an animal will generate
		void healthy();					//make an animal healthy again after paying medical cost
};
#endif
