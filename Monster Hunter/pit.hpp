#include "event.hpp"
#ifndef __PIT_HPP
#define __PIT_HPP

	class Pit : public Event {
		
		private:

		public:
			Pit();
			void action(Player&,int);
	};

#endif
