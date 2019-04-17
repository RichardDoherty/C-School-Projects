#include "level.hpp"

//initializer for leveel
Level::Level(std::ifstream& infile, int MAZE_HEIGHT, int MAZE_WIDTH):appeased(0),rows(MAZE_HEIGHT),cols(MAZE_WIDTH){
	
	//sets location of objects in the maze	
	std::string location;
	for (int r = 0; r < MAZE_HEIGHT; r++){

		std::vector<MazeLocation*> row;
		getline(infile, location); 
		for(int c = 0; c < MAZE_WIDTH; c++){	
			if(location.at(c) == '#'){	
				row.push_back(new Wall);
			} else {
				row.push_back(new OpenSpace);	
			}

			if(location.at(c) == '@'){
				row[c]->set_start();
				this->startLocation[0] = r;
				this->startLocation[1] = c;
			}

			if(location.at(c) == '^'){
				row[c]->set_end();
				row[c]->set_ladder();
			}
			
			if(location.at(c) == '%'){
				row[c]->set_professor();
			}
		}
		this->locations.push_back(row);	
	}	
	
	//randomly sets locations of the TAs and skills
	srand(time(NULL));
	int randRow, randCol;


	for(int i =  0; i < 5; i++){
		do{
			randRow = rand() % MAZE_HEIGHT;
			randCol = rand() % MAZE_WIDTH;

		}while(locations[randRow][randCol]->get_character() != ' ');	
		if(i < 2){
			this->TAs[i].setLocation(randRow,randCol);		
			locations[randRow][randCol]->move_TA();
		}else{
			locations[randRow][randCol]->progSkill();
		}
	}
}


//no dynamic memory to deallocate
Level::~Level(){}

//displays all the charaters in the level
void Level::displayLevel(){
	
	for(int n = 0; n  < locations.size(); n++){
		for(int c = 0; c < locations[n].size(); c++){
			std::cout << locations[n][c]->get_character(); 
		}
		std::cout << std::endl;
	}
}
//returns the character at a specfied location 
char Level::location(int row, int col){
	return locations[row][col]->get_character();
}

//returns the locaton of the starting position in a level
int* Level::start(){
	this->locations[startLocation[0]][startLocation[1]]->move_student();
	return this->startLocation;
	
}

//randomly changes the location of the TAs
void Level::moveTAs(){

for(int n = 0; n < 2; n++){
	int* location = this->TAs[n].getLocation(); 
	int dx, dy, direction;
	do{
		dx = 0;
		dy = 0;
		direction = rand() % 4;

		if(direction == 0)
			dx = 1;

		if(direction == 1)
			dx = -1;

		if(direction == 2)
			dy = 1;

		if(direction == 3)
			dy = -1;

	}while(this->locations[location[0] + dy][location[1] + dx]->is_occupiable() == false);
	this->locations[location[0]][location[1]]->move_TA();
	this->locations[location[0]+dy][location[1]+dx]->move_TA();
	this->TAs[n].move(dy,dx);
}
}

//test is a move is valide and then if the move is valide updates the occupation status of the locations
bool Level::testMove(std::string input, int* location){
		
	
			int dRow, dCol;
			dRow = 0;
			dCol = 0;
			
			if(input == "w")
				dRow = -1;
			

			if(input == "a")
				dCol = -1;
	
			if(input == "s")
				dRow = 1;

			if(input == "d")
				dCol = 1;
			
								
			//tests if the move is valid	
			bool possible = this->locations[location[0] + dRow][location[1] + dCol]->is_occupiable();
	
	//if possible execute the move and update occupations
	if(possible == true){
		this->locations[location[0]][location[1]]->move_student();
		this->locations[location[0]+dRow][location[1]+dCol]->move_student();
		return true;
	}else{
		//inform the user the move is invalid and return false so the user will be repromped
		std::cout << "Invalid Move\n";
		return false;
	}

}

//pick up a skill if there is one at the current location
//and return int to update the skill count
int Level::pickupSkill(int*location){
	if(this->locations[location[0]][location[1]]->has_skill() == true){
		locations[location[0]][location[1]]->removeSkill();
		return 1;
	}
	
	return 0;
}

//test if a location has a ladder
bool Level::has_ladder(int* location){
	if(this->locations[location[0]][location[1]]->has_ladder() == true){
		return true;
	}
	return false;
}

//change level of the student when they climb a ladder
void Level::levelChange(int* location){
		this->locations[location[0]][location[1]]->move_student();
}

//demonstrate a skill to the TAs
//will result in an appeasment of 10 that will ward off the TAs for 10 turns
void Level::demonstrateSkill(){
	this->appeased += 10;
	std::cout << "The TAs will be appeased for 10 turns\n";
}

//decrement the appeased count if any and display the count 
void Level::changeAppeased(){
	if(this->appeased != 0){
		--appeased;
		std::cout << "The TAs will be appeased for " << appeased << " more turns!\n";
	}
}

//check if an instructor is in a location specfied in parameters
bool Level::checkInstructor(int x, int y){
	return locations[x][y]->has_professor();
}
//check for TA a parameter specified location
bool Level::checkTA(int x,int y){
	int* TA_Location;
	for(int n = 0; n < 2; n++){
		TA_Location = TAs[n].getLocation();	
		if(TA_Location[0] == x && TA_Location[1] == y)
			return true;

		if(TA_Location[0] == x && TA_Location[1] == y - 1)
			return true;

		if(TA_Location[0] == x +1 && TA_Location[1] == y)
			return true;
		
		if(TA_Location[0] == x-1 && TA_Location[1] == y)
			return true;
		
		if(TA_Location[0] == x && TA_Location[1] == y + 1)
			return true;
	}
	return false;
	
}

//check if there is an appeased count
bool Level::isAppeased(){
	if(this->appeased > 0)
		return true;

	return false;
}

//reset all occupations on a level
void Level::reset(){
	for(int r = 0; r < rows; r++){
		for(int c = 0; c < cols; c++){
			locations[r][c]->clear();
		}
	}
	srand(time(NULL));
	int randRow, randCol;
	
	for(int i =  0; i < 5; i++){
		do{
			randRow = rand() % rows;
			randCol = rand() % cols;

		}while(locations[randRow][randCol]->get_character() != ' ');	
		if(i < 2){
			this->TAs[i].setLocation(randRow,randCol);		
			locations[randRow][randCol]->move_TA();
		}else{
			locations[randRow][randCol]->progSkill();
		}
	}
}
