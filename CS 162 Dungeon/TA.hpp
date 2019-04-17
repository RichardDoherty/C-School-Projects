#include "mazeperson.hpp"
#include <iostream>
#ifndef __TA_HPP
#define __TA_HPP

class TA: public MazePerson{
	
	public:
		TA();
		void move(int,int);
		int* getLocation();
		void setLocation(int,int);
};


#endif
