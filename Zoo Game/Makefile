

game: 	  game.cpp zoo.o monkey.o animal.o otter.o sloth.o
	  g++ game.cpp zoo.o monkey.o animal.o otter.o sloth.o -o game

zoo.o: 	  zoo.hpp zoo.cpp
	  g++ -c zoo.cpp

monkey.o: monkey.hpp monkey.cpp
	  g++ -c monkey.cpp

animal.o: animal.hpp animal.cpp
	  g++ -c animal.cpp

otter.o:  otter.hpp otter.cpp
	  g++ -c otter.cpp

slot.o:	  sloth.hpp sloth.cpp
	  g++ -c sloth.cpp
clean: 
	rm -f *.o game


