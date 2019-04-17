#include "mazeperson.hpp"
#include "level.hpp"
#ifndef __STUDENT_HPP
#define __STUDENT_HPP

class Student : public MazePerson{
	
	private:
		int progSkills;
		int currentLevel;
	public: 
		Student();
		void move(int,int);
		int* getLocation();
		void startLevel(Level);
		int level();	
		void changeLevel(int);
		void getSkill(int);
		int skills();
		void loseSkills();
		void loseSkill();
};

#endif
