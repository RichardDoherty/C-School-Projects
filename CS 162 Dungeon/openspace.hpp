#include "mazelocation.hpp"
#ifndef __OPENSPACE_HPP
#define __OPENSPACE_HPP
class OpenSpace : public MazeLocation {
	
	private: 
		char character;
		bool occupied;
		bool occupiedTA;
		bool occupiedStudent;
		bool occupiedProfessor;
		bool hasSkill;
		bool ladder;
	public: 
		OpenSpace();
		~OpenSpace();
		bool is_occupiable();
		char get_character();
		void set_start();
		void move_TA();
		void move_student();
		void set_professor();
		void set_end();
		bool has_TA();
		bool has_professor();
		bool has_student();
		bool is_start();
		bool is_end();
		void progSkill();
		void removeSkill();
		bool has_skill();
		bool has_ladder();
		void set_ladder();
		void clear();
};

#endif
