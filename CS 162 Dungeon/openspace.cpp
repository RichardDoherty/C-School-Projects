#include "openspace.hpp"

OpenSpace::OpenSpace(): character(' '), occupiedTA(false), occupiedProfessor(false), occupiedStudent(false){
	this->occupied = false;
	this->hasSkill = false;
	this->ladder = false;
}

bool OpenSpace::is_occupiable(){
	return !this->occupied;
}

char OpenSpace::get_character() {
	
	if(this->occupiedTA == true){
		return 'T';
	}
	if(this->occupiedStudent == true){
		return 'S';
	}

	return this->character;
}

void OpenSpace::set_start(){
	this->character = '@';
}

bool OpenSpace::has_skill(){
	return this->hasSkill;
}
void OpenSpace::move_TA(){
	if(occupiedTA == false){
		this->occupiedTA = true;
	}else{
		this->occupiedTA = false;
	}
}

void OpenSpace::move_student(){
	
	if(occupiedStudent == false){
		this->occupiedStudent = true;
	}else{
		this->occupiedStudent = false;
	}

}

void OpenSpace::set_end(){
	this->character = '^';
}

bool OpenSpace::has_TA(){
	return this->occupiedTA;
}

bool OpenSpace::is_start(){
	if(this->character == '@')
		return true;

	return false;
}

bool OpenSpace::is_end(){

	if(this->character == '^')
		return true;

	return false;
}

OpenSpace::~OpenSpace(){
}

bool OpenSpace::has_student(){
	return this->occupiedStudent;

}

void OpenSpace::set_professor(){
	this->character = '%';
	this->occupiedProfessor = true;
}

bool OpenSpace::has_professor(){
	return this->occupiedProfessor;
}

void OpenSpace::progSkill(){
	
	this->character = '$';
	this->hasSkill = true;
}

void OpenSpace::removeSkill(){
	this->character = ' ';
	this->hasSkill = false;
}

bool OpenSpace::has_ladder(){
	return ladder;
}

void OpenSpace::set_ladder(){
	this->ladder = true;
}

//clears all occupatons and characters except for instructor,ladder, and start
void OpenSpace::clear(){
	hasSkill = false;
	occupiedTA = false;
	occupiedStudent = false;
	if(character != '^' && character != '@' && character != '%')
		character = ' ';
}
