#include "maze.hpp"

//this function will utalize an infile to set the maze height, width and number of levels
//it will then initialize the levels appropriatlly
Maze::Maze(std::ifstream& infile){
	
	infile >> this->numLevels >> this->heightLevel >> this->widthLevel;
	std::string nextLine;
	
	getline(infile, nextLine);	
	for(int n = 0; n < this->numLevels; n++){
		levels.push_back(Level(infile,this->heightLevel,this->widthLevel));
	}

}

//function sends user to parameter supplied level in the maze
void Maze::student2Level(int){

	this->student.startLevel(this->levels[0]);
	int* position = this->student.getLocation();
	if(student.level() != 0)					//ignores character switch if on the first level because this it is already
		levels[this->student.level()].levelChange(position);	//initialize before begining while loop in main
}

Maze::~Maze(){}		//no dynamic memory to deconstruct

//function displays the current level
void Maze::displayLevel(int level){
	
	levels[level].displayLevel();		

}

//function for a users turn
//user can move, try to go up a ladder, or use a programming skill
//all the above activities are one whole turn (ie the TAs will move no matter what command you send in)
void Maze::turn(){
	std::string move;
	int dRow = 0, dCol = 0;
	int* newLocation = this->student.getLocation();	
	levels[student.level()].changeAppeased();
	std::cout << "Number of Programming Skills: " << this->student.skills() <<std::endl;
	for(int n = 0; n < numLevels; n++){
		levels[n].moveTAs();
	}
	
	do{
		//user can and w, s, a, d, u, or p as a command  anything else will result in a reprompt 
		do{
			std::cout << "\nW-Up   S-Down   A-Left   D-Right   U-climb ladder P-Demonstrate a Programming Skill\nEnter your move: ";
			std::cin >> move;
		}while(move != "w" && move != "s" && move != "a" && move != "d" && move != "u" && move != "p");
	}while(levels[this->student.level()].testMove(move,student.getLocation()) == false);
	
			if(move == "w")			//translate the w,a,s,and d to change in users position
				dRow = -1;

			if(move == "a")
				dCol = -1;
	
			if(move == "s")
				dRow = 1;

			if(move == "d")
				dCol = 1;

			//if the user entered u and is on a ladder climb the ladder and change levels
			if(move == "u" && levels[this->student.level()].has_ladder(student.getLocation())){
				student.changeLevel(this->student.level()+1);
			}

			//demonstrates programming skill and decrements studetns skill count
			if(move == "p" && student.skills() > 0){
				levels[student.level()].demonstrateSkill();
				student.loseSkill();
			}

	//move the student the specified ammount
	//both dRow and dCol will be zero if the user tried to climb a ladder or demonstrated a skill
	this->student.move(dRow,dCol);
	this->student.getSkill(levels[this->student.level()].pickupSkill(this->student.getLocation()));		
	
}

//accessor for the students current level
int Maze::studentsLevel(){
	return this->student.level();
}

//checks if the TA is in an adjacent or same location as the user
void Maze::checkTA(){
	int* location = student.getLocation();
	int x, y;
	x = location[0];
	y = location[1];
	//if they are in the same location the student loses their skills and their location is reset
	if(levels[student.level()].checkTA(x,y)== true && levels[student.level()].isAppeased() == false){
		levels[student.level()].reset();
		Maze::student2Level(student.level());
		student.loseSkills();
	}
}

//function will check if the user is in the same location as the professor
bool Maze::checkInstructor(){

	int* location = student.getLocation();
	int x, y;
	x = location[0];
	y = location[1];
	//if the user is in the same location as the professor
	if(levels[student.level()].checkInstructor(x,y)== true){
		//if they have less than 3 skills they are sent to the begining 
		//of the maze
		if(student.skills() < 3){
			levels[2].reset();
			levels[0].reset();
			Maze::student2Level(0);
			student.loseSkills();
			return false;
		}else{
			//the student passes and the game is done
			std::cout << "\nYou pass CS 162!!\n";
			return true;
		}
	}
}
