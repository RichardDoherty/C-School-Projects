#include <iostream>
#include <vector>
#include <fstream>
#include "mazelocation.hpp"
#include "openspace.hpp"
#include "wall.hpp"
#include "TA.hpp"
#include "stdlib.h"
#include "time.h"
#include "instructor.hpp"
#ifndef __LEVEL_HPP
#define __LEVEL_CPP

class Level {
	
	private:
		Instructor instructor;
		TA TAs[2];
		int* startLocation;
		int rows;
		int cols;
		std::vector<std::vector<MazeLocation*> > locations;
	protected: 
		int appeased;		//for some reason adding this member in private will result in a core dump

	public:
		Level();
		Level(std::ifstream&,int,int); 
		~Level();
		void displayLevel();
		char location(int,int);
		void placeTAs();
		int* start();
		void moveTAs();
		bool testMove(std::string, int*);
		int pickupSkill(int*);
		bool has_ladder(int*);
		void levelChange(int*);
		void demonstrateSkill();
		void changeAppeased();
		bool checkTA(int,int);
		bool isAppeased();
		void placeStudent(int*);
		void reset();
		bool checkInstructor(int,int);
};

#endif
