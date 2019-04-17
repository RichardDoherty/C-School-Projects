#include "TA.hpp"

TA::TA(){
	this->location[0] = 0;
	this->location[1] = 0;

}

void TA::move(int dRow, int dCol){
	
	this->location[0] += dRow;
	this->location[1] += dCol;

}

void TA::setLocation(int row, int col){

	this->location[0] = row;
	this->location[1] = col;
}

int* TA::getLocation(){
	return this->location;
}
