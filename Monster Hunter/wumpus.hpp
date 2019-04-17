#include "event.hpp"
#ifndef __WUMPUS_HPP
#define __WUMPUS_HPP

	class Wumpus : public Event{

		private:

		public:
			Wumpus();
			void action(Player&,int);
	};

#endif
