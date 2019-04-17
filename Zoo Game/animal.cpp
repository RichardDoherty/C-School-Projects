#include <iostream>
#include "animal.hpp"

//Parameterized constructor
//Preconditions: called by the default constructor of monkey, sloth, and otter classes
Animal::Animal(std::string name, int age, int cost,  int n_babies, int food_cost, int revenue) : 
	age(age) ,name(name), cost(cost), n_babies(n_babies), food_cost(food_cost), revenue(revenue), health(false) {age_check();}

//Return the amount of revenue an animal produces
//Preconditions: an animal has a set revenue
int Animal::animal_revenue(){
	
	return this->revenue;
}

//Returns the cost to cure a sick animal
//Preconditions: an animal has a set cost and age
int Animal::med_cost(){
	
	if (this->age < 30){
		return this->cost;
	} else {
		return (this->cost)/2;
	}	
	
}

//Returns is an animal is sick or not
//Preconditions: the animals health is initialized
bool Animal::is_sick(){
	return this->health;
}

//Will make an animals health go from false to true
//Preconditions: There is an adult animal at the zoo
void Animal::sick() {
	this->health = true;
}

//shows an animals type, age, and age status
//Preconditions: animals type age and age status have been set
void Animal::show_values() {
	int years = 0;				//animals age in years
	int days = this->age;			//animals age in days
	
	do {
		if (days > 365){		//conditional guards animals that are less than one year old from being decremented
			years++;
			days-= 365;
		}
	}while(days >= 365);			//once days is under 365 we will have the correct number of years and days

	std::cout << "Animal: " << this->name << "\tStatus: " << this->age_status << "\tAge: " << years << " years " << days << " days\n";
}

//ages an animal by one day
//Preconditions: animals age is initialized
void Animal::advance_day() {
	this->age++;				//increase age
	age_check();				//redefine an animals age status based on new age
}

//returns cost of a single animal
int Animal::animal_cost() {
	return this->cost;
}

//will change an animals age status to baby if age < 30 days, young if 30 days < age < 3 years, and adult if age >= 3 years
void Animal::age_check() {


	if (this->age < 30) {
		this->age_status = "Baby";
	} else if (this->age >= 1095) {			//3 years = 1095 days
		this->age_status = "Adult";
	} else {
		this->age_status = "Young";
	}
}

//return the cost of food for a single animal
int Animal::cost_food() {

	return this->food_cost;
}

//return an animal's age
int Animal::animal_age() {
	return this->age;
}

void Animal::healthy(){
	this->health = false;
}
