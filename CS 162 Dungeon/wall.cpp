#include "wall.hpp"

Wall::Wall(): character('#') {}

char Wall::get_character() {
	return this->character;
}

bool Wall::is_occupiable(){
	return false;
}

void Wall::progSkill(){}

void Wall::removeSkill(){}

bool Wall::has_skill(){
	return false;
}

void Wall::set_start(){}

void Wall::set_end(){}

void Wall::set_professor(){}

bool Wall::has_TA(){
	return false;
}

bool Wall::is_start(){
	return false;
}

bool Wall::is_end(){
	return false;
}

bool Wall::has_student(){
	return false;
}

bool Wall::has_professor(){
	return false;
}

void Wall::move_TA(){}

void Wall::move_student(){}

bool Wall::has_ladder(){
	return false;
}

void Wall::clear(){}

void Wall::set_ladder(){};
