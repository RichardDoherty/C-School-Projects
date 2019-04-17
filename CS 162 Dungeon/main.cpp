#include <iostream>
#include <fstream>
#include <string>
#include "maze.hpp"
int main(int argc, char** argv){
	
	//attempt to open file specified in the command line
	std::ifstream infile;
	infile.open(argv[1]);

	//if the the computer fails to open the specified file print an error message and exit the program
	if (infile.fail()){
		std::cerr << "\tERROR OPENING FILE:\nEnter the full file name into the command line " << std::endl;
		return 0;
	}

	Maze maze(infile);				//create maze from infile source
	int startLevel, x = 0;
	maze.student2Level(maze.studentsLevel());	//send student to first level
	do{
		startLevel = maze.studentsLevel();	//checks for change in level later 
		maze.displayLevel(maze.studentsLevel());//shows current level
		maze.turn();				//activate users turn
		if(startLevel != maze.studentsLevel())			//if the level has changed
			maze.student2Level(maze.studentsLevel());	//send student to new level (update current level member in student
		maze.checkTA();						//check for TA near by
	}while(maze.checkInstructor() == false);			//do while the user is not in the same spot as instructor
									//also checks for the users skills before returning true
	return 0;
}
