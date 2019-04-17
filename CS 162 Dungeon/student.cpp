#include "student.hpp"

Student::Student():progSkills(0), currentLevel(0){
	
	this->location[0] = 0;
	this->location[1] = 0;

}

void Student::move(int dRow ,int dCol){
	this->location[0] += dRow;
	this->location[1] += dCol;
}

int* Student::getLocation(){
	return this->location;
}

void Student::startLevel(Level level){
	int* start = level.start();
	this->location[0] = start[0];
	this->location[1] = start[1];
}

int Student::level(){
	return this->currentLevel;
}

void Student::changeLevel(int level){
	this->currentLevel = level;
}

void Student::getSkill(int skills){
	this->progSkills += skills;
}

int Student::skills(){
	return this->progSkills;
}

void Student::loseSkills(){
	progSkills = 0;
}

void Student::loseSkill(){
	--progSkills;
}
