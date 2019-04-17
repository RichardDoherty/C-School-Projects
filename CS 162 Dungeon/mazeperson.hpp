#ifndef __MAZEPERSON_HPP
#define __MAZEPERSON_HPP

class MazePerson {
	
	public:
		int location[2];	
	private:
		virtual void move(int,int) = 0;
};
#endif
