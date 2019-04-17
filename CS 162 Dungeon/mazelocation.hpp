#ifndef __MAZELOCATION_HPP
#define __MAZELOCATION_HPP

class	MazeLocation {
	
	public: 
		//MazeLocation();
		//virtual ~MazeLocation();
		virtual bool is_occupiable() = 0;
		virtual char get_character() = 0;
		virtual void set_end() = 0;
		virtual void set_start() = 0;
		virtual void move_TA() = 0;
		virtual void progSkill() = 0;
		virtual void move_student() = 0;
		virtual bool has_TA() = 0;
		virtual void set_professor() = 0;
		virtual bool has_professor() = 0;
		virtual bool has_student() = 0;
		virtual bool is_start() = 0;
		virtual bool is_end() = 0;
		virtual void removeSkill()= 0;
		virtual bool has_skill() = 0;
		virtual bool has_ladder() = 0;
		virtual void set_ladder() = 0;
		virtual void clear() = 0;
};



#endif
