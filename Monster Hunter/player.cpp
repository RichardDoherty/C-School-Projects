#include "player.hpp"

Player::Player(): nArrows(3), gold(false), alive(true){}

//set the position of the player
//this function is used for initially the players location and when the bats move a player
void Player::enter(int x, int y){
	this->position[0] = x;
	this->position[1] = y;
}

//accessor for the number of arrows a player has
int Player::numArrows(){
	return this->nArrows;
}

//returns if the user is still alive
bool Player::isAlive(){
	return this->alive;
}

//returns if the user has the wumpus' gold
bool Player::hasGold(){
	return this->gold;
}

//returns an array of two integers holding the x and y value of the users grid location
int* Player::currentPosition(){
	return this->position;
}

//moves a players location
//used in move function eery turn
void Player::move(int x, int y){
	this->position[0]+= x;
	this->position[1]+= y;
}

//kills the player
void Player::dead(){
	this->alive = false;
}

//gives the user the gold
void Player::foundGold(){
	this->gold = true;
}

void Player::loseArrow(){
	this->nArrows--;
}

