#include <fstream>
#include <vector>
#include "student.hpp"
#ifndef __MAZE_HPP
#define __MAZE_HPP

class Maze {
	
	private:
		int heightLevel;
		int widthLevel;
		int numLevels;
		Student student;
		std::vector<Level> levels;
	public:
		Maze(std::ifstream&);
		~Maze();
		void displayLevel(int);
		void student2Level(int);
		void turn();
		int studentsLevel();
		void checkTA();
		bool checkInstructor();
};

#endif
