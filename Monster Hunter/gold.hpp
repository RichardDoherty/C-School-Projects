#include "event.hpp"
#ifndef __GOLD_HPP
#define __GOLD_HPP

	class Gold : public Event{

		private:

		public:
			Gold();
			void action(Player&,int);
	};

#endif
