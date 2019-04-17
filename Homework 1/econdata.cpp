#include <iostream>
#include<fstream>
#include<algorithm>
#include "econdata.hpp"

struct state* allocate_states(int n_states){

	state* states = new state[n_states];
	return states;
}

void read_state_data(struct state* states, int n_states, std::ifstream& infile){


	//n_states
	//state_name state_07_unemployed state_15_unemployed state_med_income n_counties
	//county_1_name county_1_07_unemployed county_1_15_unemployed county_1_med_income
	

	for (int n = 0; n< n_states; n++){	
		infile >> states[n].name >> states[n].unemployed_2007 >> states[n].unemployed_2015 >> states[n].med_income >> states[n].n_counties;	
		struct county* counties = allocate_counties(states[n].n_counties);	
		read_county_data(counties,states[n].n_counties,infile);
		states[n].counties = counties;
	}
}

struct county* allocate_counties(int n_counties){
	
	//allocate memory for counties in state struct
	county* counties = new county[n_counties];
	return counties;
}

void read_county_data(struct county* counties, int n_counties, std::ifstream& infile) {

	//county_1_name county_1_07_unemployed county_1_15_unemployed county_1_med_income
	
	for (int n = 0; n < n_counties; n++) {
	infile >> counties[n].name >> counties[n].unemployed_2007 >> counties[n].unemployed_2015 >> counties[n].med_income;
	}
}

void free_state_data(struct state* states, int n_states){
	
	//deallocate memory that was created for states and counties
	for (int n = 0; n < n_states; n++){
		delete [] states[n].counties;	
	}
	delete [] states;	
}

//comparing functions for state values
bool cmp_med_income(const struct state& s1, const struct state& s2){
	return (s1.med_income > s2.med_income);
}


bool cmp_unemployment_2015(const struct state& s1, const struct state& s2){
	return (s1.unemployed_2015 > s2.unemployed_2015);
}


bool cmp_unemployment_2007(const struct state& s1, const struct state& s2){
	return (s1.unemployed_2007 > s2.unemployed_2007);
}

//comparing functions for county values
bool cmp_med_income_county(const struct county& s1, const struct county& s2){
	return (s1.med_income > s2.med_income);
}


bool cmp_unemployment_2015_county(const struct county& s1, const struct county& s2){
	return (s1.unemployed_2015 > s2.unemployed_2015);
}


bool cmp_unemployment_2007_county(const struct county& s1, const struct county& s2){
	return (s1.unemployed_2007 > s2.unemployed_2007);
}

//function to get the type of data the user wants to pull
std::string  get_data_type(){
	
	std::string data_type;
	do{
		//choose state or county info
		std::cout << "Would you like state or county information? \nEnter state or county: ";
		std::cin >> data_type;
	} while (data_type != "state" && data_type != "county");
	
	return data_type;
}

void state_info(std::string & stat, std::string & output, std::string & year){
		//state: choose 07' unemployment, 15' unemployment, med income
		do {
			std::cout << "\nWould you like information on median income or unemployment? \nEnter income or unemployment: " ;
			std::cin >> stat;
		} while(stat != "income" && stat != "unemployment");
		
		//unemployment: choose max, min, or descending list
		//get what type of information output the user desires
		do {
			std::cout << "\nWould you like the maximum value, minimum value, or listed statistics (median income or difference in unemployment from 2007-2015)\nEnter max, min, or list: " ;
			std::cin >> output;
		} while (output != "max" && output != "min" && output != "list");
		
		//if the user wants unemployment information ask them what year they would like
		if (stat == "unemployment" && output != "list"){
			do {
				std::cout << "\nSelect an unemployment year.\nEnter 2007 or 2015: " ;
				std::cin >> year;
			} while (year != "2007" && year != "2015");
		}
}

void county_info(std::string & stat, std::string &output, std::string & data_state, std::string & year){

		//county:choose state 
		do {
			std::cout << "\nWhat state would you like county information on?" << std::endl;	
			std::cin >> data_state;
		} while(data_state != "Oregon" && data_state != "Pennsylvania" && data_state != "California");

	
		//choose 07' unemployment, 15' unemployment, med income
		do {
			std::cout << "\nWould you like information on median income or unemployment?\nEnter income or unemployment: ";	
			std::cin >> stat;
		} while(stat != "income" && stat != "unemployment");
		
		//median: choose max, min, or descending list
		do {
			std::cout << "\nWould you like the maximum value, minimum value, or listed statistics (median income or difference in unemployment from 2007-2015)\nEnter max, min, or list: " ;
			std::cin >> output;
		} while(output != "max" && output != "min" && output != "list");


		//if the user wants unemployment information ask them what year they would like
		if (stat == "unemployment" && output != "list"){
			do {
				std::cout << "\nSelect an unemployment year.\nEnter 2007 or 2015: " ;
				std::cin >> year;
			} while (year != "2007" && year != "2015");
		}
		
}



void state_output(std::string stat, std::string output, std::string year, struct state* states, int n_states){ 

		if (stat == "income"){
			//sort the states in order of descending median income
			std::sort(states, states + n_states, cmp_med_income);

			//output max
			if (output == "max") {
				std::cout << states[0].name << " \t$" << states[0].med_income << std::endl;		
				}
		
			//output minimum
			if (output == "min") {
				std::cout << states[n_states - 1].name << " \t$" << states[n_states - 1].med_income << std::endl;		
				}
		
			//output all names and values of sorted information
			if (output == "list") {
				for (int n = 0; n < n_states; n++){
					std::cout << states[n].name << " \t$" << states[n].med_income << std::endl;		
				}
			}
		
		} else { //unemployent
			
			//sort data in order of the sepcified unemployment year
			if (year == "2015")
				std::sort(states, states + n_states, cmp_unemployment_2015);

			if (year == "2007")
				std::sort(states, states + n_states, cmp_unemployment_2007);
			
			//output name and value of max statistic
			if (output == "max") {
				std::cout << states[0].name << " \t$" << states[0].med_income  << std::endl;		
			}

			if (output == "min") {
				std::cout << states[n_states - 1].name << " \t$" << states[n_states - 1].med_income << std::endl;		
			}

			if (output == "list"){
				for (int n = 0; n < n_states; n++) {
					states[n].unemployed_2015 = states[n].unemployed_2015 - states[n].unemployed_2007;
				}

				std::sort(states, states + n_states, cmp_unemployment_2015);

				for (int n = 0; n < n_states; n++) {
					std::cout << states[n].name << "\t" << states[n].unemployed_2015 << "%" << std::endl;
				}
			}
		}
}


void county_output(std::string stat, std::string output, std::string year, int s, struct state* states){

		if (stat == "income"){
		
			//sort information in order of descending median income
			std::sort(states[s].counties, states[s].counties + states[s].n_counties, cmp_med_income_county);
			
			//display name and value of max median income
			if (output == "max"){
				std::cout << states[s].counties[0].name << " \t$" << states[s].counties[0].med_income << std::endl;
			}
			
			//display name and value of minimum median income
			if (output == "min"){
				std::cout << states[s].counties[states[s].n_counties-1].name << " \t$" << states[s].counties[states[s].n_counties-1].med_income << std::endl;
			}

			//output the names and values of all median incomes in descending order
			if (output == "list"){
				for (int n = 0; n < states[s].n_counties; n++) {
					std::cout << states[s].counties[n].name << " \t$" << states[s].counties[n].med_income << std::endl;
				}
			}
			
		} else { //unemployent
		
			if (output != "list"){
				if (year == "2007"){

					std::sort(states[s].counties, states[s].counties + states[s].n_counties, cmp_unemployment_2015_county);
					if (output =="min"){
						std::cout << states[s].counties[0].name << " \t" << states[s].counties[0].unemployed_2015 << "%" << std::endl;
					} else {
						std::cout << states[s].counties[states[s].n_counties-1].name << " \t" << states[s].counties[states[s].n_counties-1].unemployed_2015 << "%" << std::endl;
					}
				} else { //year = 2015
	
					std::sort(states[s].counties, states[s].counties + states[s].n_counties, cmp_unemployment_2007_county);
					if (output =="min"){
						std::cout << states[s].counties[0].name << " \t" << states[s].counties[0].unemployed_2007 << "%" << std::endl;
					} else {
						std::cout << states[s].counties[states[s].n_counties-1].name << " \t" << states[s].counties[states[s].n_counties-1].unemployed_2007 << "%" << std::endl;
					}
				}

			} else { //min and max condition 
			
				for (int n = 0; n < states[s].n_counties; n++) {
					states[s].counties[n].unemployed_2015 = states[s].counties[n].unemployed_2015 - states[s].counties[n].unemployed_2007;
				}

				std::sort(states[s].counties, states[s].counties + states[s].n_counties, cmp_unemployment_2015_county);

				for (int n = 0; n < states[s].n_counties; n++) {
					std::cout << states[s].counties[n].name << "\t " << states[s].counties[n].unemployed_2015 << "%" << std::endl;
				}
			}//list for diff unemployed
		}//end of unemployed condition
	}//end of county loop
