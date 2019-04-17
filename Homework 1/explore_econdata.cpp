#include <iostream>
#include<fstream>
#include<algorithm>
#include "econdata.cpp"

int main(int argc, char*argv[]){

	
	//attempt to open file specified in the command line
	std::ifstream infile;
	infile.open(argv[1]);

	//if the the computer fails to open the specified file print an error message and exit the program
	if (infile.fail()){
		std::cerr << "\tERROR OPENING FILE:\nEnter the full file name into the command line " << std::endl;
		return 0;
	}
	
	//the first value in the txt file is the number of states
	//read in the number of states into a variable n_states
	int n_states; 
	infile >> n_states; 

	//create pointer to dynamic array of state struct members and read in data 
	struct state* states = allocate_states(n_states);
	read_state_data(states,n_states,infile);

	//initially output states included in file
	std::cout << "\nStates: " << std::endl;
	for (int n = 0; n < n_states; n++){
		std::cout<< n+1 << ": " << states[n].name <<std::endl; 
	}
	
	//space the command line
	std::cout << std::endl;
	std::string data_type;


	data_type = get_data_type();


	//initialize string variables that will store user input
	std::string stat, output, data_state, year;	
	
	if (data_type == "state"){
		state_info(stat, output,year);
	} else {
		county_info(stat, output, data_state,year);
	}
	
	//set an integer to equal the index number of the state the user selected
	int s;
	for (int n = 0; n < n_states; n++){
		if (states[n].name == data_state)
			s = n;
	}

	if (data_type == "state"){

		state_output(stat, output, year, states, n_states); 
	
	} else { //county
		county_output( stat , output, year, s, states);
	}
	
	free_state_data(states, n_states);

	infile.close();
	return 0;
}
