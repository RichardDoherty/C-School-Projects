#include "player.hpp"
#include <iostream>
#ifndef __EVENT_HPP
#define __EVENT_HPP
class Event{

	protected: 
		std::string percept;
		bool empty;
		char symbol;	
	public:
		Event();
		virtual void action(Player&,int);
		virtual	void showPercept();
		virtual ~Event();
};
#endif
