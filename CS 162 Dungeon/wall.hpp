#include "mazelocation.hpp"
#ifndef __WALL_HPP
#define __WALL_HPP
	class Wall : public MazeLocation {
	
		private: 
			char character;

		public:
			Wall();
			bool is_occupiable();
			void progSkill() ;
			void removeSkill();
			char get_character();
			void set_start();
			void set_end();
			void move_TA();
			void set_professor();
			void move_student();
			bool has_TA();
			bool has_student();
			bool has_professor();
			bool is_start();
			bool is_end();
			bool has_skill();
			bool has_ladder();
			void set_ladder();
			void clear();
};

#endif
